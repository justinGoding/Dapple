#include "Application.h"

#include "memory\DoubleBufferedAllocator.h"

#include <chrono>

Application::Application() 
{
	m_Renderer = Renderer();
}

void Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	m_Renderer.Init(hInstance, nCmdShow);
}

WPARAM Application::Run() 
{
	auto start = std::chrono::high_resolution_clock::now();

	StackAllocator g_singleFrameAllocator = StackAllocator(128);
	DoubleBufferedAllocator g_doubleBufAllocator = DoubleBufferedAllocator(128);

	MSG msg;
	bool running = true;
	while (running)
	{
		// Clear the single-frame allocator's buffer every frame
		g_singleFrameAllocator.clear();

		// Swap the active and inactive buffers of the double-buffered allocator
		g_doubleBufAllocator.swapBuffers();

		// Now clear the newly active buffer, leaving last frame's
		// buffer intact
		g_doubleBufAllocator.clearCurrentBuffer();

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