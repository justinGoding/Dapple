#pragma once

#include "..\Core.h"
#include "OpenGLWindow.h"

class Renderer
{
public:
	Renderer(HINSTANCE hInstance, int nCmdShow);
	~Renderer();

	void Init();
	void OnShutdown();

	void Render(float currentTime);
private:
	void CompileShaders();
	std::string ReadFile(const std::string& filepath);
private:
	OpenGLWindow m_Window;

	GLuint m_Program;
	GLuint m_vao;
};

