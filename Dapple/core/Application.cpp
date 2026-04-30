#include "Application.h"

#include <chrono>

#include <thread>
#include <mutex>
#include <sstream>

#include "memory\DoubleBufferedAllocator.h"
#include "containers\String.h"
#include "fileIO\Win32_Files.h"

#include "containers\ThreadSafeQueue.h"
#include "containers\Vector.h"


#define PROD_THREAD_COUNT 10
#define CON_THREAD_COUNT 10
int STRING_COUNT = 50;
int strings_produced = 0;
int strings_consumed = 0;

ThreadSafeQueue<std::string> q(10);
std::thread prod_threads[PROD_THREAD_COUNT];
std::thread con_threads[CON_THREAD_COUNT];


std::mutex prod_mutex;
std::mutex con_mutex;
std::condition_variable consuming = std::condition_variable();

int increment_prod_counter()
{
	std::lock_guard lock(prod_mutex);
	return ++strings_produced;
}

void increment_con_counter()
{
	std::lock_guard lock(con_mutex);
	++strings_consumed;
}

void output_string(string str)
{
	std::cout << str << std::endl;
}

void produce_string(int thread_number)
{
	while (strings_produced < STRING_COUNT)
	{
		int string_number = increment_prod_counter();

		std::stringstream ss;
		ss << "thread " << thread_number << ": " << string_number;
		std::string tmp = ss.str();

		q.enqueue(tmp);
	}

	return;
}

void consume_string(int thread_number)
{
	while (strings_consumed < STRING_COUNT)
	{
		std::string str = q.dequeue();
		if (strings_consumed >= STRING_COUNT) return;
		str += "\n";
		std::cout << str;

		increment_con_counter();
	}

	q.close();

	return;
}



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

	for (int i = 0; i < PROD_THREAD_COUNT; i++)
	{
		prod_threads[i] = std::thread(produce_string, i);
	}

	for (int i = 0; i < CON_THREAD_COUNT; i++)
	{
		con_threads[i] = std::thread(consume_string, i);
	}

	for (int i = 0; i < PROD_THREAD_COUNT; i++)
	{
		prod_threads[i].join();
	}

	for (int i = 0; i < CON_THREAD_COUNT; i++)
	{
		con_threads[i].join();
	}

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