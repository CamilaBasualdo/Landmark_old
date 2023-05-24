#include "Logger.h"
#include <iostream>
namespace Landmark {
    namespace Debug {
        std::string Logger::ComposeMessage(std::string Contents,LogColor TextColor, LogColor Background) {
            return "[" + Origin + "] " + LogColorAnsiCode(TextColor) + LogColorAnsiCodeBackground(Background)
                + Contents + Reset + "\n";
        }
        void Logger::Log_List(std::string Title, std::vector<std::string>& Content, LogColor TextColor, LogColor Background)
        {
            std::string CompleteMessage = Title +"\n\n";
            for (auto c : Content)
                CompleteMessage += " | "+ c + "\n";
           // CompleteMessage += "+=====";
            CompleteMessage;
            Log(CompleteMessage, TextColor, Background);
        }
        void Logger::Log(std::string log, LogColor TextColor, LogColor Background)
        {

            std::cout << ComposeMessage(log, TextColor, Background);
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

        void Logger::Critical(std::string log,bool ThrowException)
        {
            Log(log, white, red);
            if (ThrowException)
                throw std::runtime_error(log);
        }
    }
}