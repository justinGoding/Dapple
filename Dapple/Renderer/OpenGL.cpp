#include "OpenGl.h"

// Loaded at window creation
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

// Loaded by call to LoadModernOpenGL()
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;

PFNGLCLEARBUFFERFVPROC glClearBufferfv = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;

PFNGLCREATEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;

PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = nullptr;