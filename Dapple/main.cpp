#include "core\Application.h"
#include "Renderer\Renderer.h"

#ifdef _DEBUG
#include "core\DebugStreamBuffer.h"
#endif
#include "core\memory\PoolAllocator.h"
#include "core\memory\DoubleEndedStackAllocator.h"

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	RedirectCoutToDebugger();
#endif
	DoubleEndedStackAllocator stack = DoubleEndedStackAllocator(sizeof(Object) * 3);

	Object* obj1 = reinterpret_cast<Object*>(stack.AllocUpper(sizeof(Object)));
	Object* obj2 = reinterpret_cast<Object*>(stack.AllocLower(sizeof(Object)));
	DoubleEndedStackAllocator::Marker marker = stack.GetUpperMarker();
	Object* obj3 = reinterpret_cast<Object*>(stack.AllocUpper(sizeof(Object)));
	Object* obj4 = reinterpret_cast<Object*>(stack.AllocLower(sizeof(Object)));

	std::cout << "Address of obj1: " << obj1 << std::endl;
	std::cout << "Address of obj2: " << obj2 << std::endl;
	std::cout << "Address of obj3: " << obj3 << std::endl;
	std::cout << "Address of obj4: " << obj4 << std::endl;
	std::cout << std::endl;

	stack.FreeUpperToMarker(marker);

	Object* obj5 = reinterpret_cast<Object*>(stack.AllocUpper(sizeof(Object)));
	Object* obj6 = reinterpret_cast<Object*>(stack.AllocLower(sizeof(Object)));

	std::cout << "Address of obj1: " << obj1 << std::endl;
	std::cout << "Address of obj2: " << obj2 << std::endl;
	std::cout << "Address of obj3: " << obj3 << std::endl;
	std::cout << "Address of obj4: " << obj4 << std::endl;
	std::cout << "Address of obj5: " << obj5 << std::endl;
	std::cout << "Address of obj6: " << obj6 << std::endl;
	std::cout << std::endl;

	stack.Clear();

	obj2 = reinterpret_cast<Object*>(stack.AllocUpper(sizeof(Object)));

	std::cout << "Address of obj2: " << obj2 << std::endl;



	/*Application app = Application::Get();
	app.Init(hInstance, nCmdShow);

	int return_msg = app.Run();

	app.OnShutdown();

	return return_msg;*/
}