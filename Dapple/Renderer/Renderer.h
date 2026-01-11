#pragma once

#include "..\math\VectorMath.h"
#include "..\math\MatrixMath.h"
#include "OpenGLWindow.h"


class Renderer
{
public:
	static void Init(HINSTANCE hInstance, int nCmdShow);
	static void OnShutdown();

	static void Render(float currentTime);
private:
	static void CompileShaders();
	static std::string ReadFile(const std::string& filepath);

	static void OnResize(int width, int height);
private:
	static OpenGLWindow Window;

	static GLuint Program;
	static GLuint vao;
	static GLuint buffer;
	static GLint mv_location;
	static GLint proj_location;

	static float aspect;
	static sfm::mat4f proj_matrix;
};

