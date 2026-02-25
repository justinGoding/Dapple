#include "OpenGLShader.h"

#include <cstdio>
#include <fstream>

GLuint OpenGLShader::load(const std::string& filepath, GLenum shader_type, bool check_errors)
{
	std::string shader_string;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != 1)
		{
			shader_string.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&shader_string[0], size);
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

	GLuint result = 0;

	result = glCreateShader(shader_type);

	if (!result)
		return 0;

	const char* shader = shader_string.c_str();
	glShaderSource(result, 1, &shader, NULL);

	glCompileShader(result);

	if (check_errors)
	{
		GLint status = 0;
		glGetShaderiv(result, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			char buffer[4096];
			glGetShaderInfoLog(result, 4096, NULL, buffer);
#ifdef _WIN32
			OutputDebugStringA(filepath.c_str());
			OutputDebugStringA(":");
			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
#else
			fprintf(stderr, "%s: %s\n", filename, buffer);
#endif
			glDeleteShader(result);
			return 0;
		}
	}

	return result;
}