#include "pch.h"
#include "CppUnitTest.h"

#include "core\containers\MaxHeap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DappleTests
{
	TEST_CLASS(DappleTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			int val = h.pop();
			Assert::AreEqual(1, val);
		}

		TEST_METHOD(TestMethod2)
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

		TEST_METHOD(TestMethod3)
		{
			MaxHeap<int> h = MaxHeap<int>();
			Assert::IsTrue(h.empty());
		}

		TEST_METHOD(TestMethod4)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			Assert::IsFalse(h.empty());
		}

		TEST_METHOD(TestMethod5)
		{
			MaxHeap<int> h = MaxHeap<int>();
			Assert::AreEqual(0, h.size());
		}

		TEST_METHOD(TestMethod6)
		{
			MaxHeap<int> h = MaxHeap<int>();
			h.push(1);
			h.push(2);
			h.push(3);
			h.push(4);
			h.push(5);
			Assert::AreEqual(5, h.size());
		}

		TEST_METHOD(TestMethod7)
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

		TEST_METHOD(TestMethod8)
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

		TEST_METHOD(TestMethod9)
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
