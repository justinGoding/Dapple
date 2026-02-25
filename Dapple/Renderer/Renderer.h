#pragma once

#include "..\math\Math.h"
#include "OpenGL\OpenGLWindow.h"
#include "sb7\sb7object.h"
#include "sb7\sb7ktx.h"

struct Uniforms
{
	GLint mvp;
};

struct uniforms_block
{
	sfm::mat4f mv_matrix;
	sfm::mat4f view_matrix;
	sfm::mat4f proj_matrix;
};

struct Textures
{
	GLuint color;
	GLuint normals;
};

class Renderer
{
public:
	Renderer();

	void Init(HINSTANCE hInstance, int nCmdShow);
	void OnShutdown();

	void Render(float currentTime);
private:
	void CompileShaders();
	std::string ReadFile(const std::string& filepath);

	static void OnResize(int width, int height);
	static void OnKeyEvent(int key, int action);

	void PrintShaderLog(GLuint shader);

	void GenerateTexture(float* data, int width, int height);
private:
	OpenGLWindow m_Window;

	GLuint m_clear_program;
	GLuint m_append_program;
	GLuint m_resolve_program;

	Textures m_textures;

	GLuint m_uniforms_buffer;

	Uniforms m_uniforms;

	sb7::object m_object;

	GLuint m_fragment_buffer;
	GLuint m_head_pointer_image;
	GLuint m_atomic_counter_buffer;
	GLuint m_dummy_vao;

	static float aspect;
	static sfm::mat4f proj_matrix;
};

