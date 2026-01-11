#include "Renderer.h"

#include <cmath>
#include <fstream>

OpenGLWindow Renderer::Window = OpenGLWindow();

GLuint Renderer::Program = 0;
GLuint Renderer::vao = 0;
GLuint Renderer::buffer = 0;
GLint Renderer::mv_location = 0;
GLint Renderer::proj_location = 0;

float Renderer::aspect = 0.0f;
sfm::mat4f Renderer::proj_matrix;

void Renderer::Init(HINSTANCE hInstance, int nCmdShow)
{
	Window.Init(hInstance);
	Window.SetWindowSizeCallback(OnResize);
	if (!Window.Create(hInstance, nCmdShow))
	{
		PostQuitMessage(1);
	}

	CompileShaders();

	mv_location = glGetUniformLocation(Program, "mv_matrix");
	proj_location = glGetUniformLocation(Program, "proj_matrix");

	// First create and bind a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat vertex_positions[] =
	{
		-0.25f,  0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		 0.25f, -0.25f, -0.25f,

		 0.25f, -0.25f, -0.25f,
		 0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,

		 0.25f, -0.25f, -0.25f,
		 0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f, -0.25f,

		 0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,
		 0.25f,  0.25f, -0.25f,

		 0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		 0.25f, -0.25f,  0.25f,
		 0.25f, -0.25f, -0.25f,

		 0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f,  0.25f,

		-0.25f,  0.25f, -0.25f,
		 0.25f,  0.25f, -0.25f,
		 0.25f,  0.25f,  0.25f,

		 0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f, -0.25f
	};

	// Now generate some data and put it in a buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

	// Setup our vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::OnShutdown()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(Program);

	Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	static const GLfloat one = 1.0f;

	glViewport(0, 0, Window.m_config.width, Window.m_config.height);
	glClearBufferfv(GL_COLOR, 0, blue);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(Program);

	glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj_matrix);

	for (int i = 0; i < 24; i++)
	{
		float f = (float)i + (float)currentTime * 0.3f;

		sfm::mat4f camera = translation(0.0f, 0.0f, -6.0f);

		sfm::mat4f rot1 = rotation(0.0174532925f * ((float)currentTime * 45.0f), 0.0f, 1.0f, 0.0f);
		sfm::mat4f rot2 = rotation(0.0174532925f * ((float)currentTime * 21.0f), 1.0f, 0.0f, 0.0f);

		float x = sinf(2.1f * f) * 2.0f;
		float y = cosf(1.7f * f) * 2.0f;
		float z = sinf(1.3f * f) * cosf(1.5f * f) * 2.0f;
		sfm::mat4f translate = translation(x, y, z);

		sfm::mat4f mv_matrix = translate * rot2 * rot1 * camera;

		// Set the model-view and projection matrices
		glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);

		// Draw 6 faces of 2 triangles of 3 vertices each = 36 vertices
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	SwapBuffers(Window.GetDeviceContext());
}

void Renderer::CompileShaders()
{
	static std::string vs_source = ReadFile("Renderer/Shaders/vertex.glsl");

	/*static std::string tcs_source = ReadFile("Renderer/Shaders/tessellationControl.glsl");

	static std::string tes_source = ReadFile("Renderer/Shaders/tessellationEval.glsl");

	static std::string gs_source = ReadFile("Renderer/Shaders/geometry.glsl");*/

	static std::string fs_source = ReadFile("Renderer/Shaders/fragment.glsl");
	
	Program = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* sourceCStr = vs_source.c_str();
	glShaderSource(vs, 1, &sourceCStr, NULL);
	glCompileShader(vs);

	/*GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	sourceCStr = tcs_source.c_str();
	glShaderSource(tcs, 1, &sourceCStr, NULL);
	glCompileShader(tcs);

	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	sourceCStr = tes_source.c_str();
	glShaderSource(tes, 1, &sourceCStr, NULL);
	glCompileShader(tes);

	GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
	sourceCStr = gs_source.c_str();
	glShaderSource(gs, 1, &sourceCStr, NULL);
	glCompileShader(gs);*/

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	sourceCStr = fs_source.c_str();
	glShaderSource(fs, 1, &sourceCStr, NULL);
	glCompileShader(fs);

	glAttachShader(Program, vs);
	/*glAttachShader(m_Program, tcs);
	glAttachShader(m_Program, tes);
	glAttachShader(m_Program, gs);*/
	glAttachShader(Program, fs);

	glLinkProgram(Program);
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