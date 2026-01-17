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
GLuint Renderer::tex_wall = 0;

GLuint Renderer::tex_ceiling = 0;
GLuint Renderer::tex_floor = 0;

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

	tex_wall = sb7::ktx::file::load("C:\\dev\\Dapple\\Dapple\\Renderer\\media\\textures\\brick.ktx");
	tex_ceiling = sb7::ktx::file::load("C:\\dev\\Dapple\\Dapple\\Renderer\\media\\textures\\ceiling.ktx");
	tex_floor = sb7::ktx::file::load("C:\\dev\\Dapple\\Dapple\\Renderer\\media\\textures\\floor.ktx");

	int i;
	GLuint textures[] = { tex_floor, tex_wall, tex_ceiling };

	for (i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindVertexArray(render_vao);
}

void Renderer::OnShutdown()
{
	glDeleteProgram(render_prog);

	Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	static const GLfloat ones[] = { 1.0f };

	glViewport(0, 0, Window.m_config.width, Window.m_config.height);
	glClearBufferfv(GL_COLOR, 0, black);

	glUseProgram(render_prog);

	sfm::mat4f proj_matrix = perspective(60.0f DEG, (float)Window.m_config.width / (float)Window.m_config.height, 0.1f, 100.0f);

	glUniform1f(uniforms.offset, currentTime * 0.003f);

	GLuint textures[] = { tex_wall, tex_floor, tex_wall, tex_ceiling };
	for (int i = 0; i < 4; i++)
	{
		float angle = 90.0f DEG * (float)i;
		sfm::mat4f mv_matrix = scale(30.0f, 1.0f, 1.0f) *
			rotation(90.0f DEG, 0.0f, 1.0f, 0.0f) *
			translation(-0.5f, 0.0f, -10.0f) *
			rotation(90.0f DEG * (float)i, sfm::vec3f(0.0f, 0.0f, 1.0f));
		sfm::mat4f mvp = mv_matrix * proj_matrix;

		glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, mvp);

		glBindTexture(GL_TEXTURE_2D, textures[i]);
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

	uniforms.mvp = glGetUniformLocation(render_prog, "mvp");
	uniforms.offset = glGetUniformLocation(render_prog, "offset");
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
			tex_floor++;
			if (tex_floor > 1) tex_floor = 0;
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
