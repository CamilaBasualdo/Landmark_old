#include "Logger.h"
#include <iostream>
#include "Debug/LogSystem.h"
namespace Landmark {

	std::string Logger::Old_ComposeMessage(std::string Contents) {
		return "[" + Origin + "] " + Contents + "\n";
	}
	std::string Logger::Old_ComposeMessageANSI(std::string Contents, LogColor TextColor, LogColor Background) {
		return "[" + Origin + "] " + LogColorAnsiCode(TextColor) + LogColorAnsiCodeBackground(Background)
			+ Contents + Reset + "\n";
	}
	void Logger::Log_List(std::string Title, const std::vector<std::string>& Content, LogColor TextColor, LogColor Background)
	{
		std::string CompleteMessage = Title + "\n\n";
		for (auto c : Content)
			CompleteMessage += " | " + c + "\n";
		// CompleteMessage += "+=====";
		CompleteMessage;
		Log(CompleteMessage, TextColor, Background);
	}
	void Logger::Log(std::string log, LogColor TextColor, LogColor Background)
	{
		
		std::cout << Old_ComposeMessageANSI(log, TextColor, Background);
		LogSystem::Log(Old_ComposeMessage(log));
	}

	void Logger::Debug(std::string log)
	{
		Log(log, white, black);
	}

	void Logger::Warning(std::string log)
	{
		Log(log, yellow, black);
	}

	void Logger::Error(std::string log)
	{
		Log(log, bright_red, black);
	}

	void Logger::Critical(std::string log, bool ThrowException)
	{
		Log(log, white, red);
		if (ThrowException)
		{
			LogSystem::Log2File();
			throw std::runtime_error(log);
			
		}
			
	}

}