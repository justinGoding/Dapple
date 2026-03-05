#include "core\Application.h"
#include "Renderer\Renderer.h"

#ifdef _DEBUG
#include "core\DebugStreamBuffer.h"
#endif
#include "core\memory\PoolAllocator.h"
#include "core\memory\StackAllocator.h"
#include "core\containers\List.h"

#include <iostream>

// The Object structure uses custom allocator
// overloading 'new' and 'delete' operators
struct Object
{
	// Object data, 16 bytes
	uint64_t data[4];

	// Declare our custom allocator for
	// the Object structure
	static PoolAllocator allocator;

	static void* operator new(size_t size)
	{
		return allocator.alloc(size);
	}

	static void operator delete(void* ptr, size_t size)
	{
		return allocator.dealloc(ptr, size);
	}
};

PoolAllocator Object::allocator{ 8 };

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	RedirectCoutToDebugger();
#endif
	List<int> list = List<int>();
	list.insert(1);
	list.insert(2);
	list.insert(3);
	list.insert(4);
	list.insert(5);
	list.insert(6);
	list.insert(7);
	list.insert(8);
	list.insert(9);
	
	for (auto i : list)
	{
		std::cout << i << std::endl;
	}
	std::cout << std::endl;
	/*list.print();
	std::cout << std::endl;

	list.removeFront();
	list.removeFront();

	list.print();
	std::cout << std::endl;*/

	/*Application app = Application::Get();
	app.Init(hInstance, nCmdShow);

	int return_msg = app.Run();

	app.OnShutdown();

	return return_msg;*/
}