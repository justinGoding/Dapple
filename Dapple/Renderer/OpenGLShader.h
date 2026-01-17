#pragma once

#include "OpenGL.h"

class OpenGLShader
{
public:
	static GLuint load(const std::string& filepath, GLenum shader_type = GL_FRAGMENT_SHADER,
#ifdef _DEBUG
		bool check_errors = true);
#else
		bool check_errors = false);
#endif
};