#include "core\Application.h"

#ifdef _DEBUG
#include "core\DebugStreamBuffer.h"
#endif

#include "core\memory\PoolAllocator.h"
#include "core\memory\StackAllocator.h"
#include "core\containers\List.h"
#include "core\containers\HashMap.h"
#include "core\containers\String.h"

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
	HashMap<uint64_t, int> map = HashMap<uint64_t, int>();
	map.insert("Wait"_sid, 1);
	map.insert("until"_sid, 2);
	map.insert("you"_sid, 3);
	map.insert("see"_sid, 4);
	map.insert("me"_sid, 5);
	map.insert("on"_sid, 6);
	map.insert("my"_sid, 7);
	map.insert("bike"_sid, 8);


	map.at("Wait"_sid);
	map.at("until"_sid);
	map.at("you"_sid);
	map.at("see"_sid);
	map.at("me"_sid);
	map.at("on"_sid);
	map.at("my"_sid);

	Vector<std::pair<uint64_t, int>> contents = map.contents();

	for (auto [k, v] : contents)
	{
		std::cout << k << ": " << v << std::endl;
	}
	std::cout << std::endl;


	/*list.print();
	std::cout << std::endl;

	list.removeFront();
	list.removeFront();

	list.print();
	std::cout << std::endl;*/

	//Application app = Application::Get();
	//app.Init(hInstance, nCmdShow);

	//int return_msg = app.Run();

	//app.OnShutdown();

	//return return_msg;
}