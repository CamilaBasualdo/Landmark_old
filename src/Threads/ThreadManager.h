#pragma once
#include <functional>
#include <list>
#include "../Module/Module.h"
#include "../Logger.h"
#include "Thread.h"
namespace Landmark::Threads
{
	class Thread;
	class ThreadManager : Module
	{
		static inline std::list<Thread>_Threads = {};
		static inline Logger LOGGER = Logger("Thread Manager");

	public:

		static Thread* CreateThread(std::string Name,std::function<void()> function)
		{
			//_Threads.emplace_back(Name, function);
			_Threads.push_back(Thread(Name,function));
			LOGGER.Log("Thread Created ("+Name+")");
			return &_Threads.back();
			//return &_Threads.back();
			//return new Thread(std::forward<Function>(f),std::forward<Args>(args...).);
		}
		
		void PreInit() override;
		void Init() override;
		void PostInit() override;


		void PostExit() override;
	};

	
}
