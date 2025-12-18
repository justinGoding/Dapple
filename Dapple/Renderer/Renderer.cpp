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

void Renderer::Render()
{
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, blue);

	glUseProgram(m_Program);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	SwapBuffers(m_Window.GetDeviceContext());
}

void Renderer::CompileShaders()
{
	static const char* vs_source[] =
	{
		"#version 460 core													\n"
		"																	\n"
		"void main(void)													\n"
		"{																	\n"
		"	const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),	\n"
		"									 vec4(-0.25, -0.25, 0.5, 1.0),	\n"
		"									 vec4( 0.25,  0.25, 0.5, 1.0));	\n"
		"	gl_Position = vertices[gl_VertexID];							\n"
		"}																	\n"
	};

	static const char* fs_source[] =
	{
		"#version 460 core						\n"
		"										\n"
		"out vec4 color;						\n"
		"										\n"
		"void main(void)						\n"
		"{										\n"
		"	color = vec4(0.0, 0.8, 1.0, 1.0);	\n"
		"}										\n"
	};

	m_Program = glCreateProgram();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	glAttachShader(m_Program, vs);
	glAttachShader(m_Program, fs);

	glLinkProgram(m_Program);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
}
