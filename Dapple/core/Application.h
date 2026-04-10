#pragma once

#include <thread>

#include "Core.h"

#include "..\renderer\Renderer.h"

class Application
{
private:
	Application();

public:
	void Init(HINSTANCE hInstance, int nCmdShow);
	WPARAM Run();
	void Render(float currentTime);
	void OnShutdown();

	non_instanced Application& Get() 
	{ 
		// TODO: replace new with internal memory allocation
		local_persist Application* s_Instance = new Application();
		return *s_Instance; 
	}

private:
	Renderer m_Renderer;
};