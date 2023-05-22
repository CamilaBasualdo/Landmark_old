#pragma once
#include <string>


#include "Logger.h"

namespace Landmark {
    class Engine;
    namespace Debug {
        class Debugger {
            friend Engine;
        protected:
            void Init();
        public:
            static Logger GetLogger(std::string Origin);
        };
    }
}
