#pragma once

#include <gl/gl.h>
#include "GLHeaders/glext.h"
#include "GLHeaders/wglext.h"
#include "GLHeaders/glcorearb.h"

#pragma comment (lib, "opengl32.lib")

static bool ModernOpenGLLoaded = false;

// Loaded at window creation
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

// Loaded by call to LoadModernOpenGL()

bool LoadModernOpenGL()
{
	if (ModernOpenGLLoaded) return true;

	return true;
}