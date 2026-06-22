#pragma once

#include <thread>

#include "Core.h"

#include "..\renderer\Renderer.h"
#include "memory\DoubleBufferedAllocator.h"
#include "Clock.h"

class Application
{
private:
	Application();

public:
	void Init(HINSTANCE hInstance, int nCmdShow);
	WPARAM Run();
	void Render(float currentTime);
	void OnShutdown();

	void updateGame();
	void updateFrameTime();

	non_instanced Application& Get() 
	{ 
		local_persist Application* s_Instance = new Application();
		return *s_Instance; 
	}

private:
	Renderer m_Renderer;

	StackAllocator m_singleFrameAllocator = StackAllocator(128);
	DoubleBufferedAllocator m_doubleBufAllocator = DoubleBufferedAllocator(128);

	Clock m_clock;
	Timestamp m_lastFrameTime;
};