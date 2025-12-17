#include "OpenGLWindow.h"

#include "Dbt.h"
#include <vector>

bool OpenGLWindow::OpenGLFunctionsLoaded = false;

OpenGLWindow::OpenGLWindow()
{
	m_config.width = 1024;
	m_config.height = 720;
	m_config.posX = CW_USEDEFAULT;
	m_config.posY = 0;
	m_config.windowed = true;
	m_style = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
	m_exStyle = WS_EX_APPWINDOW;
}

OpenGLWindow::~OpenGLWindow()
{
	Destroy();
}

bool OpenGLWindow::Create(HINSTANCE hInstance, int nCmdShow)
{
	// Create temporary window
	HWND tempWND = CreateWindowExW(
		m_exStyle, m_windowClass,
		L"Temporary Window",
		m_style,
		0, 0,							// position x, y
		1, 1,							// width, height
		NULL, NULL,						// parent window, menu
		GetModuleHandleW(NULL), NULL);	// instance, param
	if (tempWND == NULL)
	{
		LastWin32Error();
		return false;
	}

	HDC tempDC = GetDC(tempWND);	// Device Context
	if (tempDC == NULL)
	{
		LastWin32Error();
		return false;
	}

	PIXELFORMATDESCRIPTOR tempPFD;
	ZeroMemory(&tempPFD, sizeof(tempPFD));
	tempPFD.nSize = sizeof(tempPFD);
	tempPFD.nVersion = 1;
	tempPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	tempPFD.iPixelType = PFD_TYPE_RGBA;
	tempPFD.cColorBits = 24;
	tempPFD.cAlphaBits = 8;
	tempPFD.cDepthBits = 24;

	const int tempPFDID = ChoosePixelFormat(tempDC, &tempPFD);
	if (tempPFDID == 0)
	{
		ShowMessage(L"ChoosePixelFormat() failed");
		return false;
	}

	if (SetPixelFormat(tempDC, tempPFDID, &tempPFD) == false)
	{
		ShowMessage(L"SetPixelFormat() failed");
		return false;
	}

	HGLRC tempRC = wglCreateContext(tempDC);	// Rendering Context

	if (tempRC == 0)
	{
		ShowMessage(L"wglCreateContext() failed");
		return false;
	}

	if (wglMakeCurrent(tempDC, tempRC) == false)
	{
		ShowMessage(L"wglMakeCurrent() failed");
		return false;
	}

	// get pointers to opengl functions needed for making extended pixel format
	wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == nullptr)
	{
		ShowMessage(L"wglGetProcAddress() failed to get wglChoosePixelFormatARB");
		return false;
	}

	wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == nullptr)
	{
		ShowMessage(L"wglGetProcAddress() failed to get wglCreateContextAttribsARB");
		return false;
	}

	if (m_config.windowed == true)
	{
		AdjustSize();
		Center();
	}

	// create a new window and context
	m_WND = CreateWindowExW(
		m_exStyle, m_windowClass,
		L"OpenGL Window",
		m_style,
		0, 0,							// position x, y
		1, 1,							// width, height
		NULL, NULL,						// parent window, menu
		GetModuleHandleW(NULL), NULL);	// instance, param
	if (m_WND == NULL) LastWin32Error();

	ChangeWindowMessageFilterEx(m_WND, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(m_WND, WM_COPYDATA, MSGFLT_ALLOW, NULL);

	// Adjust window rect to account for DPI scaling of the window frame and
	// if enabled, DPI scaling of the content area
	// This cannot be done until we know what monitor the window was placed on
	RECT rect = { 0, 0, m_config.width, m_config.height };
	WINDOWPLACEMENT wp = { sizeof(wp) };

	ClientToScreen(m_WND, (POINT*)&rect.left);
	ClientToScreen(m_WND, (POINT*)&rect.right);

	AdjustWindowRectExForDpi(&rect, m_style, FALSE, m_exStyle, GetDpiForWindow(m_WND));

	// Only update the restored window rect as the window may be maximized
	GetWindowPlacement(m_WND, &wp);
	wp.rcNormalPosition = rect;
	wp.showCmd = SW_HIDE;
	SetWindowPlacement(m_WND, &wp);

	DragAcceptFiles(m_WND, TRUE);

	m_DC = GetDC(m_WND);
	if (m_DC == NULL) LastWin32Error();

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int pixelFormatID; UINT numFormats;
	const bool status = wglChoosePixelFormatARB(m_DC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (!status || numFormats == 0)
	{
		ShowMessage(L"wglChoosePixelFormatARB() failed");
		return false;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(m_DC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(m_DC, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 6;
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0
	};

	m_RC = wglCreateContextAttribsARB(m_DC, 0, contextAttribs);
	if (m_RC == NULL)
	{
		ShowMessage(L"wglCreateContextAttribsARB() failed");
		return false;
	}

	// delete temporary context and window

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempRC);
	ReleaseDC(tempWND, tempDC);
	DestroyWindow(tempWND);
	if (!wglMakeCurrent(m_DC, m_RC))
	{
		ShowMessage(L"wglMakeCurrent() failed");
		return false;
	}

	if (!LoadModernOpenGL())
	{
		ShowMessage(L"Failed to load OpenGL functions");
		return false;
	}

	if (!SetWindowTextA(m_WND, reinterpret_cast<LPCSTR>(glGetString(GL_VERSION))))
		LastWin32Error();

	/*STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	GetStartupInfo(&si);*/
	ShowWindow(m_WND, nCmdShow);

	return true;
}

void OpenGLWindow::Center()
{
	RECT primaryDisplaySize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryDisplaySize, 0); // system taskbar and application desktop toolbars not included
	m_config.posX = (primaryDisplaySize.right - m_config.width) / 2;
	m_config.posY = (primaryDisplaySize.bottom - m_config.height) / 2;
}

void OpenGLWindow::Destroy()
{
	wglMakeCurrent(NULL, NULL);
	if (m_RC) wglDeleteContext(m_RC);
	if (m_DC) ReleaseDC(m_WND, m_DC);
	if (m_WND) DestroyWindow(m_WND);
}

bool OpenGLWindow::Init(HINSTANCE hInstance)
{
	// Register window class
	m_windowClass = MAKEINTATOM(RegisterOpenGLClass(hInstance));
	if (m_windowClass == 0)
	{
		ShowMessage(L"Win32: failed to register window class");
		return false;
	}

	if (!CreateHelperWindow(hInstance)) return false;

	PollMonitors();

	return false;
}

ATOM OpenGLWindow::RegisterOpenGLClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = GetModuleHandleW(NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = L"OpenGL Window";

	// TODO: Load application icon

	return RegisterClassEx(&wcex);
}

LRESULT OpenGLWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

bool OpenGLWindow::CreateHelperWindow(HINSTANCE hInstance)
{
	MSG msg;

	m_helperWND = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		m_windowClass,
		L"message window",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 1, 1,
		NULL, NULL,
		GetModuleHandleW(NULL),
		NULL);

	if (!m_helperWND)
	{
		ShowMessage(L"Win32: Failed to create helper window");
		return false;
	}

	// HACK: The command to the first ShowWindow call is ignored if the parent
	// process passed along a STARTUPINFO, so clear that with a no-op call
	ShowWindow(m_helperWND, SW_HIDE);

	// Register for HID device notification
	{
		DEV_BROADCAST_DEVICEINTERFACE_W dbi;
		ZeroMemory(&dbi, sizeof(dbi));
		dbi.dbcc_size = sizeof(dbi);
		dbi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		dbi.dbcc_classguid = { 0x4d1e55b2,0xf16f,0x11cf,{0x88,0xcb,0x00,0x11,0x11,0x00,0x00,0x30} };

		m_deviceNotificationHandle = RegisterDeviceNotificationW(m_helperWND, (DEV_BROADCAST_HDR*)&dbi, DEVICE_NOTIFY_WINDOW_HANDLE);
	}

	while (PeekMessageW(&msg, m_helperWND, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return true;
}

void OpenGLWindow::PollMonitors()
{
	// TODO win32 get connected monitors
}

void OpenGLWindow::AdjustSize()
{
	RECT rect = { 0, 0, m_config.width, m_config.height };
	AdjustWindowRectExForDpi(&rect, m_style, FALSE, m_exStyle, 96);	// 96 is user default screen dpi
	m_config.width = rect.right - rect.left;
	m_config.height = rect.bottom - rect.top;
}

void OpenGLWindow::GetWindowSize()
{
	RECT area;
	GetClientRect(m_WND, &area);
	m_config.width = area.right;
	m_config.height = area.bottom;
}

void OpenGLWindow::ShowMessage(LPCWSTR message)
{
	MessageBoxW(m_WND, message, L"OpenGLWindow", MB_ICONERROR);
}

void OpenGLWindow::LastWin32Error()
{
	// Retrieve the system error message for the last error code
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL) == 0)
	{
		MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("Error"), MB_OK);
		ExitProcess(dw);
	}

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	ExitProcess(dw);
}
