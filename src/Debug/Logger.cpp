#include "Logger.h"
#include <iostream>
namespace Landmark {
    namespace Debug {
        void Logger::Log_List(std::string Title, std::initializer_list<std::string> Content, LogColor TextColor, LogColor Background)
        {

        }
        void Logger::Log(std::string log, LogColor TextColor, LogColor Background)
        {

            std::cout << "[" << Origin << "] " << "\033[1;" << static_cast<int>(TextColor) << "m" << "\033[1;" << static_cast<int>(Background) + 10 << "m"
                 << log << Reset << "\n";
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