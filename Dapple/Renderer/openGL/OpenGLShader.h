#pragma once

#include <string>

#include "..\core\Core.h"
#include "OpenGL.h"

class OpenGLShader
{
public:
	non_instanced GLuint load(const std::string& filepath, GLenum shader_type = GL_FRAGMENT_SHADER,
#ifdef _DEBUG
		bool check_errors = true);
#else
		bool check_errors = false);
#endif
};