#pragma once

#include "OpenGL.h"

typedef void (*WindowSizeFunction)(int width, int height);
typedef void (*KeyEventFunction)(int key, int action);

class OpenGLWindow
{
public:
	OpenGLWindow();
	~OpenGLWindow();

	bool Init(HINSTANCE hInstance);
	bool Create(HINSTANCE hInstance, int nCmdShow);

	void Center();
	HDC GetDeviceContext() { return m_DC; }
	void Destroy();

	void SetWindowSizeCallback(WindowSizeFunction callback) { m_WindowSizeCallback = callback; }
	void SetKeyEventCallback(KeyEventFunction callback) { m_KeyEventCallback = callback; }

private:
	ATOM RegisterTempClass(HINSTANCE hInstance);
	ATOM RegisterOpenGLClass(HINSTANCE hInstance);
	static LRESULT CALLBACK TempWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool CreateHelperWindow(HINSTANCE hInstance);
	void PollMonitors();
	
	void AdjustSize();
	void GetWindowSize();
	void ShowMessage(LPCWSTR message);
	void LastWin32Error();


public:
	static bool OpenGLFunctionsLoaded;

	LPTSTR m_windowClass;
	HWND m_helperWND;
	HGLRC m_RC;
	HDC m_DC;
	HWND m_WND;
	DWORD m_style, m_exStyle;

	HDEVNOTIFY m_deviceNotificationHandle;

	struct Config
	{
		int width, height;
		int posX, posY;
		bool windowed;
	} m_config;

	struct Monitor
	{
		std::string name;
		void* userPointer;

		// Physical dimensions in millimeters
		int widthMM, heightMM;

		// The window whose video mode is current on this monitor
		HWND window;
	} m_monitor;
	uint8_t m_monitorCount = 0;

public:
	WindowSizeFunction m_WindowSizeCallback = nullptr;
	KeyEventFunction m_KeyEventCallback = nullptr;
};

