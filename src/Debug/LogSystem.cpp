#include "LogSystem.h"
#include <fstream>
#include <iostream>
#include <fstream>
#include <filesystem>

void Landmark::LogSystem::_LoggingThreadInit()
{
	LOGGER.Log("Init");
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(250));

		if (Logs.empty()) continue;
		else Log2File();
	} while (!ShutdownThread);

	

}

void Landmark::LogSystem::Log2File()
{
	std::unique_lock<std::mutex> lock(LogFileMutex);
	std::queue<std::string> Logs_Copy;

	Logs.swap(Logs_Copy);

	lock.unlock();

	while (!Logs_Copy.empty())
	{
		logFile << Logs_Copy.front();
		Logs_Copy.pop();
	}
				
	logFile.flush();
	
}

Landmark::LogSystem::LogSystem()
{
	
}

void Landmark::LogSystem::Init()
{
	_LoggingThread = std::thread(_LoggingThreadInit);
}

void Landmark::LogSystem::Shutdown()
{
	ShutdownThread = true;
	_LoggingThread.join();
	Log2File();
	logFile.close();
	LOGGER.Log("Shutdown");
}

void Landmark::LogSystem::Log(std::string _log)
{
	std::unique_lock<std::mutex> lock(LogFileMutex);
	Logs.push(_log);
	lock.unlock();
	
}
