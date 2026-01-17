#include "Renderer.h"

#include "OpenGLShader.h"

#include <cmath>
#include <fstream>

OpenGLWindow Renderer::Window = OpenGLWindow();

GLuint Renderer::Program = 0;
GLuint Renderer::vao = 0;
GLuint Renderer::buffer = 0;
GLint Renderer::mv_location = 0;
GLint Renderer::proj_location = 0;
GLuint Renderer::texture = 0;

GLuint Renderer::render_prog = 0;
GLuint Renderer::tex_object[2];
GLuint Renderer::tex_index = 0;

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

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
	static const GLubyte tex_data[] =
	{
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
		W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
	};
#undef B
#undef W

	glGenTextures(1, &tex_object[0]);
	glBindTexture(GL_TEXTURE_2D, tex_object[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	tex_object[1] = sb7::ktx::file::load("C:\\dev\\Dapple\\Dapple\\Renderer\\media\\textures\\pattern1.ktx");

	object.load("C:\\dev\\Dapple\\Dapple\\Renderer\\media\\objects\\torus_nrms_tc.sbm");

	CompileShaders();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::OnShutdown()
{
	glDeleteProgram(Program);
	glDeleteTextures(2, tex_object);

	Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	static const GLfloat ones[] = { 1.0f };

	glClearBufferfv(GL_COLOR, 0, blue);
	glClearBufferfv(GL_DEPTH, 0, ones);

	glViewport(0, 0, Window.m_config.width, Window.m_config.height);

	glBindTexture(GL_TEXTURE_2D, tex_object[tex_index]);

	glUseProgram(render_prog);

	sfm::mat4f proj_matrix = perspective(60.0f DEG, (float)Window.m_config.width / (float)Window.m_config.height, 0.1f, 1000.0f);

	sfm::mat4f mv_matrix = rotation((float)currentTime * 21.1f DEG, 0.0f, 0.0f, 1.0f) *
							rotation((float)currentTime * 19.3f DEG, 0.0f, 1.0f, 0.0f) *
							translation(0.0f, 0.0f, -3.0f);

	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);

	object.render();

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

	uniforms.mv_matrix = glGetUniformLocation(render_prog, "mv_matrix");
	uniforms.proj_matrix = glGetUniformLocation(render_prog, "proj_matrix");
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
	proj_matrix = perspective(0.873f, aspect, 0.1f, 1000.0f);
}

void Renderer::OnKeyEvent(int key, int action)
{
	if (action)
	{
		if (key == 0x54)
		{
			tex_index++;
			if (tex_index > 1) tex_index = 0;
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
