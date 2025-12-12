
#include "Core.h"
#include "Renderer\OpenGLWindow.h"

OpenGLWindow window;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	window.Init(hInstance);
	if (!window.Create(hInstance, nCmdShow))
	{
		PostQuitMessage(1);
	}

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
		window.Render();
	}
	window.Destroy();

	return msg.wParam;
}