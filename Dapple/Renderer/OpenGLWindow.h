#pragma once

#include <windows.h>
#include <string>

class OpenGLWindow
{
public:
	OpenGLWindow();
	~OpenGLWindow();

	bool Init(HINSTANCE hInstance);
	bool Create(HINSTANCE hInstance, int nCmdShow);

	void Center();
	void Render();
	void Destroy();

private:

	ATOM RegisterOpenGLClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool CreateHelperWindow(HINSTANCE hInstance);
	void PollMonitors();
	
	void AdjustSize();
	void GetWindowSize();
	void ShowMessage(LPCWSTR message);
	void LastWin32Error();

private:
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
};

