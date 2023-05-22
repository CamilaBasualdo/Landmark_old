#include "Debug.h"
#include "Logger.h"
#ifdef _WIN32
#include <Windows.h>
#include <cstdio>
#endif
namespace Landmark {
    namespace Debug {
        void Debugger::Init()
        {
#ifdef _WIN32 // Windows
            AllocConsole();

            // Get the standard output handle
            HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);

            // Redirect cout to the console window
            FILE* fileOut;
            freopen_s(&fileOut, "CONOUT$", "w", stdout);
            *stdout = *fileOut;

            // Get the standard input handle
            HANDLE handleIn = GetStdHandle(STD_INPUT_HANDLE);

            // Redirect cin to the console window
            FILE* fileIn;
            freopen_s(&fileIn, "CONIN$", "r", stdin);
            *stdin = *fileIn;
#elif __APPLE__ // macOS
            system("osascript -e 'tell app \"Terminal\" to do script \"cd \\\"$(pwd)\\\"\"'");
#elif __linux__ // Linux
            system("gnome-terminal -- sh -c 'cd \"$(pwd)\"; exec bash'");
#endif
        }
        Logger Debugger::GetLogger(std::string Origin)
        {
            return Logger(Origin);
        }
    }
}