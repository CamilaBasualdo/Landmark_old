#pragma once
#include "VK/Vulkan.h"
#include "Logic/RenderLogic.h"

namespace Landmark
{
    class Render::RenderLogic;
    struct EngineInitParameters {
        bool DebugMode = false;
        const char* AppName = "Default App Name";
        
    };
    class Engine {



    public:
        static void Init(EngineInitParameters p);
    };
}
