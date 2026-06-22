#include "pch.h"
#include "CppUnitTest.h"

#include <thread>
#include <mutex>

#include <atomic>
#include <array>

#include <optional>

#include "core\containers\ThreadSafeQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DappleTests
{
	constexpr int prod_thread_count = 10;
	constexpr int con_thread_count = 10;

	constexpr int INTEGER_COUNT = 50;
	int integers_produced = 0;
	int integers_consumed = 0;

	ThreadSafeQueue<int> q(10);
	std::thread prod_threads[prod_thread_count];
	std::thread con_threads[con_thread_count];

	std::mutex prod_mutex;
	std::mutex con_mutex;
	std::condition_variable consuming = std::condition_variable();

	std::array<std::atomic<int>, INTEGER_COUNT> arr{};
	bool thread_results[con_thread_count] = { false };

	int exchange_element(size_t idx, int val)
	{
		return arr[idx].exchange(val, std::memory_order_relaxed);
	}

	int increment_prod_counter()
	{
		std::lock_guard lock(prod_mutex);
		int val = integers_produced;
		++integers_produced;
		return val;
	}

	void increment_con_counter()
	{
		std::lock_guard lock(con_mutex);
		++integers_consumed;
	}

	void produce_int()
	{
		while (integers_produced < INTEGER_COUNT)
		{
			int val = increment_prod_counter();
			q.enqueue(val);
		}

		return;
	}

	void consume_int(int thread_number)
	{
		thread_results[thread_number] = true;

		while (integers_consumed < INTEGER_COUNT)
		{
			std::optional<int> item = q.dequeue();
			if (integers_consumed >= INTEGER_COUNT) return;

			int val;
			if (item)
				val = item.value();
			else
			{
				thread_results[thread_number] = false;
				return;
			}

			int check = exchange_element(val, val);
			if (check != 0)
			{
				thread_results[thread_number] = false;
				return;
			}

			increment_con_counter();
		}

		q.close();

		thread_results[thread_number] = true;
		return;
	}

	TEST_CLASS(DappleTests)
	{
	public:

		TEST_METHOD(TSQTest1)
		{
			for (int i = 0; i < prod_thread_count; i++)
			{
				prod_threads[i] = std::thread(produce_int);
			}

			for (int i = 0; i < con_thread_count; i++)
			{
				con_threads[i] = std::thread(consume_int, i);
			}

			
			for (int i = 0; i < prod_thread_count; i++)
			{
				prod_threads[i].join();
			}

			for (int i = 0; i < con_thread_count; i++)
			{
				con_threads[i].join();
			}

			Assert::AreEqual(INTEGER_COUNT, integers_consumed);

			for (int i = 0; i < INTEGER_COUNT; i++)
			{
				int val = arr[i].load(std::memory_order_relaxed);
				Assert::AreEqual(i, val);
			}

			for (int i = 0; i < con_thread_count; i++)
			{
				Assert::IsTrue(thread_results[i]);
			}
		}

	};
}