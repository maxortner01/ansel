#pragma once

#include <thread>
#include <vector>

namespace Ansel
{
	class Thread
	{
		std::thread* thread;
		bool finished = false;

		template<typename... Args>
		void wrapperFunction(std::function<void(Args...)> func, Args... args) {
			func(args...);

			finished = true;
		}

	public:
		template<typename... Args>
		Thread(std::function<void(Args...)> func, Args... args) {
			start(func, args...);
		}

		bool isFinished() {
			return finished;
		}

		template<typename... Args>
		void start(std::function<void(Args...)> func, Args... args) {
			std::function<void(std::function<void(Args...)>, Args...)> wrapper = &Thread::wrapperFunction;
			thread = new std::thread(wrapper, this, func, args...);
		}

		~Thread() {
			delete thread;
		}
	};

	class ASync
	{		
		static std::vector<Thread*> threads;

	public:

		template<typename... Args>
		static Thread* makeThread(std::function<void(Args...)> func, Args... args) {
			Thread* thread = new Thread(func, args...);
			threads.push_back(thread);
			return thread;
		}
	};
}