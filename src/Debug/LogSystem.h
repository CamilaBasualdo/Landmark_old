#pragma once
#include "../module/Module.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <string>

#include "../Logger.h"

namespace Landmark
{
	class Logger;
	class LogSystem
	{
		friend class Engine;
		friend class Logger;
		static inline std::mutex LogFileMutex;
		static inline std::ofstream logFile = std::ofstream("log.txt", std::ios::out | std::ios::trunc);

		static inline std::thread _LoggingThread;
		static inline std::queue<std::string> Logs;
		static inline bool ShutdownThread = false;
		static void _LoggingThreadInit();

		static inline Logger LOGGER = Logger("LogSystem");
		static void Log2File();

	protected:
		LogSystem();

		static void Init();
		static void Shutdown();
		static void Log(std::string _log);
	};

}
