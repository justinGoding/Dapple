#include "Renderer.h"

#include "OpenGLShader.h"

#include <cmath>
#include <fstream>

OpenGLWindow Renderer::Window = OpenGLWindow();

GLuint Renderer::render_prog = 0;
GLuint Renderer::render_vao = 0;
GLuint Renderer::buffer = 0;
GLint Renderer::mv_location = 0;
GLint Renderer::proj_location = 0;

GLuint Renderer::tex_alien_array;
GLuint Renderer::rain_buffer;

float Renderer::droplet_x_offset[256];
float Renderer::droplet_rot_speed[256];
float Renderer::droplet_fall_speed[256];

Uniforms Renderer::uniforms;

sb7::object Renderer::object = sb7::object();

float Renderer::aspect = 0.0f;
sfm::mat4f Renderer::proj_matrix;

void Renderer::Init(HINSTANCE hInstance, int nCmdShow)
{
	Window.Init(hInstance);
	Window.SetWindowSizeCallback(OnResize);
	Window.SetKeyEventCallback(OnKeyEvent);
	if (!Window.Create(hInstance, nCmdShow))
	{
		PostQuitMessage(1);
	}

	CompileShaders();

	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);

	tex_alien_array = sb7::ktx::file::load("Renderer/media/textures/aliens.ktx");
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenBuffers(1, &rain_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, rain_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(float) * 4, NULL, GL_DYNAMIC_DRAW);

	for (int i = 0; i < 256; i++)
	{
		droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
		droplet_rot_speed[i] = (random_float() + 0.5f)* ((i & 1) ? -3.0f : 3.0f);
		droplet_fall_speed[i] = random_float() + 0.2f;
	}

	glBindVertexArray(render_vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::OnShutdown()
{
	glDeleteProgram(render_prog);

	Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	float t = currentTime;

	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	static const GLfloat ones[] = { 1.0f };

	glViewport(0, 0, Window.m_config.width, Window.m_config.height);
	glClearBufferfv(GL_COLOR, 0, black);

	glUseProgram(render_prog);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, rain_buffer);
	sfm::vec4f* droplet = (sfm::vec4f*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 256 * sizeof(float) * 4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < 256; i++)
	{
		droplet[i][0] = droplet_x_offset[i];
		droplet[i][1] = 2.0f - fmodf((t + float(i)) * droplet_fall_speed[i], 4.31f);
		droplet[i][2] = t * droplet_rot_speed[i];
		droplet[i][3] = 0.0f;
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	int alien_index;
	for (alien_index = 0; alien_index < 256; alien_index++)
	{
		glVertexAttribI1i(0, alien_index);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	SwapBuffers(Window.GetDeviceContext());
}

void Renderer::CompileShaders()
{
	if (render_prog)
		glDeleteProgram(render_prog);

	GLuint vs, fs;

	vs = OpenGLShader::load("C:\\dev\\Dapple\\Dapple\\Renderer\\Shaders\\vertex.glsl", GL_VERTEX_SHADER);
	fs = OpenGLShader::load("C:\\dev\\Dapple\\Dapple\\Renderer\\Shaders\\fragment.glsl", GL_FRAGMENT_SHADER);
	
	render_prog = glCreateProgram();
	glAttachShader(render_prog, vs);
	glAttachShader(render_prog, fs);
	glLinkProgram(render_prog);

	glDeleteShader(vs);
	glDeleteShader(fs);
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
