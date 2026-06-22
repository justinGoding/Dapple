#include "Application.h"

#include <sstream>
#include <cmath>

#include "Config.h"
#include "containers\String.h"
#include "fileIO\Files.h"

#include "containers\Vector.h"

// Trying -p because Mina told me to

#if AVERAGE_FRAME_TIME == true
float frameTimes[10] = { 33.333f, 33.333f, 33.333f, 33.333f, 33.333f, 33.333f, 33.333f, 33.333f, 33.333f, 33.333f };
float totalTimeOfFrames = 33.333f * 10.0f;
int frameTimesIndex = 0;
#endif

void output_string(string str)
{
	std::cout << str << std::endl;
}

Application::Application() 
{
	m_Renderer = Renderer();
	m_clock = Clock();
}

void Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	m_clock._init();
	m_lastFrameTime = m_clock.timestamp();
	m_Renderer.Init(hInstance, nCmdShow);
}

WPARAM Application::Run() 
{
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

		updateGame();
	}

	return msg.wParam;
}

void Application::Render(float currentTime)
{
	m_Renderer.Render(currentTime);
}

void Application::OnShutdown() {}

void Application::updateGame()
{
	updateFrameTime();

	// Clear the single-frame allocator's buffer every frame
	m_singleFrameAllocator.clear();

	// Swap the active and inactive buffers of the double-buffered allocator
	m_doubleBufAllocator.swapBuffers();

	// Now clear the newly active buffer, leaving last frame's
	// buffer intact
	m_doubleBufAllocator.clearCurrentBuffer();

	m_Renderer.Render(m_clock.totalElapsedTime() / 1000.0f);
}

void Application::updateFrameTime()
{
	Duration frameDuration = m_clock.elapsedTime(m_lastFrameTime);
#if GOVERN_FRAMERATE == true
	if (frameDuration < TARGET_FRAME_TIME)
	{
		float remaining_time = TARGET_FRAME_TIME - frameDuration;
		std::chrono::milliseconds sleep_time = round<std::chrono::milliseconds>(std::chrono::duration<float, std::milli>(remaining_time));
		std::this_thread::sleep_for(sleep_time);
	}
	else if (frameDuration > TARGET_FRAME_TIME)
	{
		float remaining_time = fmod(frameDuration, TARGET_FRAME_TIME);
		std::chrono::milliseconds sleep_time = round<std::chrono::milliseconds>(std::chrono::duration<float, std::milli>(remaining_time));
		std::this_thread::sleep_for(sleep_time);
	}

	m_clock._update();
	frameDuration = m_clock.elapsedTime(m_lastFrameTime);
	m_lastFrameTime = m_clock.timestamp();
#endif

#if AVERAGE_FRAME_TIME == true
	totalTimeOfFrames -= frameTimes[frameTimesIndex];
	totalTimeOfFrames += m_clock.deltaTime();
	frameTimes[frameTimesIndex] = m_clock.deltaTime();
	frameTimesIndex = (frameTimesIndex + 1) % 10;

	float deltaTime = totalTimeOfFrames / 10;
#else
	float deltaTime = m_clock.deltaTime();
	std::cout << deltaTime << "ms" << std::endl;
#endif
}
