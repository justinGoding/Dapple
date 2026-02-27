#include "core\Application.h"
#include "Renderer\Renderer.h"

#ifdef _DEBUG
#include "core\DebugStreamBuffer.h"
#endif
#include "core\memory\PoolAllocator.h"

#include <iostream>

// The Object structure uses custom allocator
// overloading 'new' and 'delete' operators
struct Object
{
	// Object data, 16 bytes
	uint64_t data[2];

	// Declare our custom allocator for
	// the Object structure
	static PoolAllocator allocator;

	static void* operator new(size_t size)
	{
		return allocator.allocate(size);
	}

	static void operator delete(void* ptr, size_t size)
	{
		return allocator.deallocate(ptr, size);
	}
};

PoolAllocator Object::allocator{ 8 };

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	RedirectCoutToDebugger();
#endif
	// Allocate 10 pointers to our Object instances
	constexpr int arraySize = 10;

	Object* objects[arraySize];

	// Two uint64_t, 16 bytes
	std::cout << "About to allocate " << arraySize << " objects" << std::endl;

	for (int i = 0; i < arraySize; ++i)
	{
		objects[i] = new Object();
		std::cout << "new [" << i << "] = " << objects[i] << std::endl;
	}

	std::cout << std::endl;

	// Deallocate all the objects
	for (int i = arraySize - 1; i >= 0; --i)
	{
		std::cout << "delete [" << i << "] = " << objects[i] << std::endl;
		delete objects[i];
	}

	std::cout << std::endl;

	// New objects reuses previous block
	objects[0] = new Object();
	std::cout << "new [0] = " << objects[0] << std::endl << std::endl;

	/*Application app = Application::Get();
	app.Init(hInstance, nCmdShow);

	int return_msg = app.Run();

	app.OnShutdown();

	return return_msg;*/
}