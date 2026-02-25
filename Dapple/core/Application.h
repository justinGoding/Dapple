#pragma once

#include "Core.h"
#include "..\Renderer\Renderer.h"

class Application
{
private:
	Application();

public:
	void Init(HINSTANCE hInstance, int nCmdShow);
	int Run();
	void Render(float currentTime);
	void OnShutdown();

	static Application& Get() 
	{ 
		// TODO: replace new with internal memory allocation
		static Application* s_Instance = new Application();
		return *s_Instance; 
	}

private:
	Renderer m_Renderer;
};