#include "core\Application.h"
#include "Renderer\Renderer.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Application app = Application::Get();
	app.Init(hInstance, nCmdShow);

	int return_msg = app.Run();

	app.OnShutdown();

	return return_msg;
}