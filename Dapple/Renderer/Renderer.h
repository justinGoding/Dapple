#pragma once

#include "..\math\Math.h"
#include "OpenGLWindow.h"
#include "sb7\sb7object.h"
#include "sb7\sb7ktx.h"

struct Uniforms
{
	GLint mvp;
	GLint offset;
};

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
	static void OnKeyEvent(int key, int action);

	static void PrintShaderLog(GLuint shader);

	static void GenerateTexture(float* data, int width, int height);
private:
	static OpenGLWindow Window;

	static GLuint render_prog;
	static GLuint render_vao;
	static GLuint buffer;
	static GLint mv_location;
	static GLint proj_location;

	static GLuint tex_wall;
	static GLuint tex_ceiling;
	static GLuint tex_floor;

	static Uniforms uniforms;

	static sb7::object object;

	static float aspect;
	static sfm::mat4f proj_matrix;
};

