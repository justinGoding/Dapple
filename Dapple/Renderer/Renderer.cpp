#include <cmath>
#include <fstream>

#include "Renderer.h"

Renderer::Renderer(HINSTANCE hInstance, int nCmdShow)
{
	m_Window.Init(hInstance);
	if (!m_Window.Create(hInstance, nCmdShow))
	{
		PostQuitMessage(1);
	}
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
	CompileShaders();
}

void Renderer::OnShutdown()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteProgram(m_Program);

	m_Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };

	glClearBufferfv(GL_COLOR, 0, blue);

	glUseProgram(m_Program);

	glDrawArrays(GL_PATCHES, 0, 3);

	SwapBuffers(m_Window.GetDeviceContext());
}

void Renderer::CompileShaders()
{
	static std::string vs_source = ReadFile("Renderer/Shaders/vertex.glsl");

	static std::string tcs_source = ReadFile("Renderer/Shaders/tessellationControl.glsl");

	static std::string tes_source = ReadFile("Renderer/Shaders/tessellationEval.glsl");

	static std::string fs_source = ReadFile("Renderer/Shaders/fragment.glsl");
	
	m_Program = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* sourceCStr = vs_source.c_str();
	glShaderSource(vs, 1, &sourceCStr, NULL);
	glCompileShader(vs);

	GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	sourceCStr = tcs_source.c_str();
	glShaderSource(tcs, 1, &sourceCStr, NULL);
	glCompileShader(tcs);

	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	sourceCStr = tes_source.c_str();
	glShaderSource(tes, 1, &sourceCStr, NULL);
	glCompileShader(tes);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	sourceCStr = fs_source.c_str();
	glShaderSource(fs, 1, &sourceCStr, NULL);
	glCompileShader(fs);

	glAttachShader(m_Program, vs);
	glAttachShader(m_Program, tcs);
	glAttachShader(m_Program, tes);
	glAttachShader(m_Program, fs);

	glLinkProgram(m_Program);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
