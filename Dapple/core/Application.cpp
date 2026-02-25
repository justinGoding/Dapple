#include "Application.h"

#include <chrono>

Application::Application() 
{
	m_Renderer = Renderer();
}

void Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	m_Renderer.Init(hInstance, nCmdShow);
}

int Application::Run() 
{
	auto start = std::chrono::high_resolution_clock::now();

	MSG msg;
	bool running = true;
	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		auto time = std::chrono::high_resolution_clock::now();
		m_Renderer.Render(std::chrono::duration<float, std::milli>(time - start).count() / 1000.0f);
	}

	return msg.wParam;
}

void Application::Render(float currentTime)
{
	m_Renderer.Render(currentTime);
}
void Application::OnShutdown() {}