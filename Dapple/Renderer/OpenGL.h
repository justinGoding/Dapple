#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "GLHeaders/glext.h"
#include "GLHeaders/wglext.h"
#include "GLHeaders/glcorearb.h"

#pragma comment (lib, "opengl32.lib")

#define LOAD_GL_FUNC(x, p) x = reinterpret_cast<p>(wglGetProcAddress(#x)); \
					if (x == nullptr) { MessageBoxA(NULL, #x, "Error loading OpenGL function", MB_OK); return false;}

static bool ModernOpenGLLoaded = false;

// Loaded at window creation
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

// Loaded by call to LoadModernOpenGL()
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

extern PFNGLCLEARBUFFERFVPROC glClearBufferfv;
extern PFNGLUSEPROGRAMPROC glUseProgram;

extern PFNGLCREATEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

static bool LoadModernOpenGL()
{
	if (ModernOpenGLLoaded) return true;

	LOAD_GL_FUNC(glCreateProgram, PFNGLCREATEPROGRAMPROC);
	LOAD_GL_FUNC(glCreateShader, PFNGLCREATESHADERPROC);
	LOAD_GL_FUNC(glShaderSource, PFNGLSHADERSOURCEPROC);
	LOAD_GL_FUNC(glCompileShader, PFNGLCOMPILESHADERPROC);
	LOAD_GL_FUNC(glAttachShader, PFNGLATTACHSHADERPROC);
	LOAD_GL_FUNC(glLinkProgram, PFNGLLINKPROGRAMPROC);
	LOAD_GL_FUNC(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);
	LOAD_GL_FUNC(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);

	LOAD_GL_FUNC(glClearBufferfv, PFNGLCLEARBUFFERFVPROC);
	LOAD_GL_FUNC(glUseProgram, PFNGLUSEPROGRAMPROC);

	LOAD_GL_FUNC(glDeleteVertexArrays, PFNGLCREATEVERTEXARRAYSPROC);
	LOAD_GL_FUNC(glDeleteProgram, PFNGLDELETEPROGRAMPROC);

	ModernOpenGLLoaded = true;
	return true;
}