#include <iostream>

#include "core\Application.h"

#ifdef _DEBUG
#include "core\DebugStreamBuffer.h"
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	RedirectCoutToDebugger();
#endif

	Application app = Application::Get();
	app.Init(hInstance, nCmdShow);

	int return_msg = app.Run();

	app.OnShutdown();

	return return_msg;
}