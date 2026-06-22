#include "pch.h"
#include "CppUnitTest.h"

#include "core\containers\MaxHeap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DappleTests
{
	TEST_CLASS(DappleTests)
	{
	public:
		
		TEST_METHOD(MaxHeapTest1)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			int val = h.pop();
			Assert::AreEqual(1, val);
		}

		TEST_METHOD(MaxHeapTest2)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(2);
			h.push(3);
			h.push(5);
			h.push(7);
			h.push(6);
			h.push(1);
			int val = h.pop();
			Assert::AreEqual(7, val);
		}

		TEST_METHOD(MaxHeapTest3)
		{
			MaxHeap<int> h = MaxHeap<int>();
			Assert::IsTrue(h.empty());
		}

		TEST_METHOD(MaxHeapTest4)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			Assert::IsFalse(h.empty());
		}

		TEST_METHOD(MaxHeapTest5)
		{
			MaxHeap<int> h = MaxHeap<int>();
			Assert::AreEqual(0, h.size());
		}

		TEST_METHOD(MaxHeapTest6)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			h.push(2);
			h.push(3);
			h.push(4);
			h.push(5);
			Assert::AreEqual(5, h.size());
		}

		TEST_METHOD(MaxHeapTest7)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			h.push(2);
			h.push(3);
			h.push(4);
			h.push(5);
			h.pop();
			Assert::AreEqual(4, h.size());
		}

		TEST_METHOD(MaxHeapTest8)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			h.push(2);
			h.push(3);
			h.push(4);
			h.push(5);
			h.pop();
			h.pop();
			h.push(4);
			Assert::AreEqual(4, h.size());
		}

		TEST_METHOD(MaxHeapTest9)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			h.push(2);
			h.push(3);
			h.push(4);
			h.push(5);
			h.push(6);
			h.push(7);
			h.push(8);
			h.push(9);
			bool val = h.push(10);
			Assert::IsTrue(val);
			val = h.push(11);
			Assert::IsFalse(val);
		}

	};
}
