#pragma once
#include <functional>
#include <string>
#include <thread>
namespace Landmark::Threads
{
	class ThreadManager;
	class Thread
	{
		friend ThreadManager;
		std::thread _SystemThread;
		const std::string Name;
		
	protected:
		bool CalledToShutdown = false;



		
		Thread(std::string _name,std::function<void()> function) : Name(_name),_SystemThread(function)
		{
			
		};
	public:
		bool GetShouldShutdown();
		void Join();
		bool Joinable();
	};
}

