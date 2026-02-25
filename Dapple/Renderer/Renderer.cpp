#include "Renderer.h"

#include "sb7/shader.h"
#include "OpenGL\OpenGLShader.h"

#include <cmath>
#include <fstream>

float Renderer::aspect = 0.0f;
sfm::mat4f Renderer::proj_matrix;

Renderer::Renderer() 
{
	m_Window = OpenGLWindow();

	m_clear_program = 0;
	m_append_program = 0;
	m_resolve_program = 0;

	m_object = sb7::object();
}

void Renderer::Init(HINSTANCE hInstance, int nCmdShow)
{
	m_Window.Init(hInstance);
	m_Window.SetWindowSizeCallback(OnResize);
	m_Window.SetKeyEventCallback(OnKeyEvent);
	if (!m_Window.Create(hInstance, nCmdShow))
	{
		PostQuitMessage(1);
	}

	CompileShaders();

	glGenBuffers(1, &m_uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 4 * 4 * 3, NULL, GL_DYNAMIC_DRAW);

	m_object.load("Renderer/media/objects/dragon.sbm");

	glGenBuffers(1, &m_fragment_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_fragment_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 1024 * 1024 * 16, NULL, GL_DYNAMIC_COPY);

    glGenBuffers(1, &m_atomic_counter_buffer);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_atomic_counter_buffer);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, 4, NULL, GL_DYNAMIC_COPY);

    glGenTextures(1, &m_head_pointer_image);
    glBindTexture(GL_TEXTURE_2D, m_head_pointer_image);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, 1024, 1024);

    glGenVertexArrays(1, &m_dummy_vao);
    glBindVertexArray(m_dummy_vao);
}

void Renderer::OnShutdown()
{
	glDeleteProgram(m_clear_program);
	glDeleteProgram(m_append_program);
	glDeleteProgram(m_resolve_program);

	m_Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	const float f = currentTime;

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	static const GLfloat ones[] = { 1.0f };

	glViewport(0, 0, m_Window.m_config.width, m_Window.m_config.height);
	//glClearBufferfv(GL_COLOR, 0, black);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(m_clear_program);
	glBindVertexArray(m_dummy_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(m_append_program);

	sfm::mat4f model_matrix = scale(7.0f);
	sfm::vec3f view_position = sfm::vec3f(cosf(f * 0.35f) * 120.0f, cosf(f * 0.4f) * 30.0f, sinf(f * 0.35f) * 120.0f);
	sfm::mat4f view_matrix = lookat(view_position,
		sfm::vec3f(0.0f, 30.0f, 0.0f),
		sfm::vec3f(0.0f, 1.0f, 0.0f));

	sfm::mat4f mv_matrix = model_matrix * view_matrix;
	sfm::mat4f proj_matrix = perspective(50.0f DEG,
		(float)m_Window.m_config.width / (float)m_Window.m_config.height,
		0.1f,
		1000.0f);

	glUniformMatrix4fv(m_uniforms.mvp, 1, GL_FALSE, mv_matrix * proj_matrix);

	static const unsigned int zero = 0;
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_atomic_counter_buffer);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_fragment_buffer);

	glBindImageTexture(0, m_head_pointer_image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	m_object.render();

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(m_resolve_program);

	glBindVertexArray(m_dummy_vao);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	SwapBuffers(m_Window.GetDeviceContext());
}

void Renderer::CompileShaders()
{
	GLuint  shaders[2];

	shaders[0] = OpenGLShader::load("Renderer/media/shaders/fragmentlist/clear.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = OpenGLShader::load("Renderer/media/shaders/fragmentlist/clear.fs.glsl", GL_FRAGMENT_SHADER);

	if (m_clear_program)
		glDeleteProgram(m_clear_program);

	m_clear_program = sb7::program::link_from_shaders(shaders, 2, true);

	shaders[0] = OpenGLShader::load("Renderer/media/shaders/fragmentlist/append.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = OpenGLShader::load("Renderer/media/shaders/fragmentlist/append.fs.glsl", GL_FRAGMENT_SHADER);

	if (m_append_program)
		glDeleteProgram(m_append_program);

	m_append_program = sb7::program::link_from_shaders(shaders, 2, true);

	m_uniforms.mvp = glGetUniformLocation(m_append_program, "mvp");

	shaders[0] = OpenGLShader::load("Renderer/media/shaders/fragmentlist/resolve.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = OpenGLShader::load("Renderer/media/shaders/fragmentlist/resolve.fs.glsl", GL_FRAGMENT_SHADER);

	if (m_resolve_program)
		glDeleteProgram(m_resolve_program);

	m_resolve_program = sb7::program::link_from_shaders(shaders, 2, true);
}

std::string Renderer::ReadFile(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != 1)
		{
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&result[0], size);
			in.close();
		}
		else
		{
			MessageBoxW(NULL, L"Could not read file", L"Dapple", MB_ICONERROR);
		}
	}
	else
	{
		MessageBoxW(NULL, L"Could not open file", L"Dapple", MB_ICONERROR);
	}
	return result;
}

void Renderer::OnResize(int width, int height)
{
	aspect = (float)width / float(height);
	proj_matrix = perspective(0.873f, aspect, 0.1f, 100.0f);
}

void Renderer::OnKeyEvent(int key, int action)
{
	if (action)
	{
		if (key == 0x54)
		{
			
		}
	}
}

void Renderer::PrintShaderLog(GLuint shader)
{
	std::string str;
	GLint len;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len != 0)
	{
		str.resize(len);
		glGetShaderInfoLog(shader, len, NULL, &str[0]);
	}

	OutputDebugStringA(str.c_str());
}

void Renderer::GenerateTexture(float* data, int width, int height)
{
	int x, y;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
			data[(y * width + x) * 4 + 3] = 1.0f;
		}
	}
}
