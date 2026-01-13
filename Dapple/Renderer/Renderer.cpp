#include "Renderer.h"

#include <cmath>
#include <fstream>

OpenGLWindow Renderer::Window = OpenGLWindow();

GLuint Renderer::Program = 0;
GLuint Renderer::vao = 0;
GLuint Renderer::buffer = 0;
GLint Renderer::mv_location = 0;
GLint Renderer::proj_location = 0;
GLuint Renderer::texture = 0;

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

	// Generate a name for the texture
	glGenTextures(1, &texture);

	// Now bind it to the context using the GL_TEXTURE_2D binding point
	glBindTexture(GL_TEXTURE_2D, texture);

	// Specify the amount of storage we want to use for the texture
	glTexStorage2D(
		GL_TEXTURE_2D,	// 2D texture
		8,				// 8 mipmap levels
		GL_RGBA32F,		// 32-bit floating-point RGBA data
		256, 256		// 256 x 256 texels
	);

	// Define some data to upload into the texture
	float* data = new float[256 * 256 * 4];

	// generate_texture() is a function that fills memory with image data
	GenerateTexture(data, 256, 256);

	// Assume the texture is already bound to the GL_TEXTURE_2D target
	glTexSubImage2D(
		GL_TEXTURE_2D,	// 2D texture
		0,				// Level 0
		0, 0,			// Offset 0, 0
		256, 256,		// 256 x 256 texels, replace entire image
		GL_RGBA,		// Four channel data
		GL_FLOAT,		// Floating-point data
		data			// Pointer to data
	);

	// Free the memory we allocated before - \GL now has our data
	delete[] data;

	CompileShaders();
}

void Renderer::OnShutdown()
{
	glDeleteProgram(Program);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &texture);

	Window.Destroy();
}

void Renderer::Render(float currentTime)
{
	static const GLfloat blue[] = { 0.129f, 0.586f, 0.949f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, blue);

	glUseProgram(Program);
	glDrawArrays(GL_TRIANGLES, 0, 3);

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

	PrintShaderLog(vs);

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

	PrintShaderLog(fs);

	glAttachShader(Program, vs);
	/*glAttachShader(m_Program, tcs);
	glAttachShader(m_Program, tes);
	glAttachShader(m_Program, gs);*/
	glAttachShader(Program, fs);

	glLinkProgram(Program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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
