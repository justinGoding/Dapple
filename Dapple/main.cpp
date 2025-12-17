
#include "Core.h"
#include "Renderer\Renderer.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Renderer renderer = Renderer(hInstance, nCmdShow);
	renderer.Init();

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
		renderer.Render();
	}
	renderer.OnShutdown();

	return msg.wParam;
}