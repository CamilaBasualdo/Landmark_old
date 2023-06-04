#include "ThreadManager.h"



namespace Landmark::Threads 
{
	//std::list<Thread> ThreadManager::_Threads = {};


	std::string ThreadManager::GetName() const
	{
		return "ThreadManager";
	}

	void ThreadManager::PreInit()
	{
		LOGGER.Log("init");
	}

	void ThreadManager::Init()
	{
	}

	void ThreadManager::PostInit()
	{
	}

	void ThreadManager::PostExit()
	{
		for (Thread& thr : _Threads)
		{
			LOGGER.Log("Joining Thread (" + thr.Name + ")");
			thr.CalledToShutdown = true;
			thr.Join();
		}
		LOGGER.Log("All Threads Closed.");
	}
}
