#pragma once
#include <string>
namespace Landmark {
    namespace Debug {
        class Debugger;
        class Logger {
            friend Debugger;
            std::string Origin;
            const std::string Reset = "\033[0m";
        protected:
            Logger(std::string _Origin) {
                Origin = _Origin;
            }

        public:
            enum LogColor {
                black = 30,
                red = 31,
                green = 32,
                yellow = 33,
                blue = 34,
                magenta = 35,
                cyan = 36,
                white = 37,
                bright_black = 90,
                bright_red = 91,
                bright_green = 92,
                bright_yellow = 93,
                bright_blue = 94,
                bright_magenta = 95,
                bright_cyan = 96,
                bright_white = 97
            };

            void Log_List(std::string Title, std::initializer_list<std::string> Content, LogColor TextColor = white, LogColor Background = black);
            void Log(std::string log, LogColor TextColor = white, LogColor Background = black);

            void Debug(std::string log);
            void Warning(std::string log);
            void Error(std::string log);
            void Critical(std::string log, bool ThrowException = false);
        };
    }
}
