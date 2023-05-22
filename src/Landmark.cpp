#include "Landmark.h"

#include "Debug/Debug.h"
#include "Module/Module.h"
namespace Landmark
{

	void Engine::Init(EngineInitParameters p)
	{
		
		auto logger = Debug::Debugger::GetLogger("Landmark");

		logger.Log("Test Log", logger.white, logger.green);
		logger.Debug("Test Debug");
		logger.Warning("Test Warning");
		logger.Error("Test Error");
		logger.Critical("Test Critical\n");

		logger.Debug("Engine Init!");
		Vk::VulkanInitParameters VkParams;
		VkParams.AppName = p.AppName;
		VkParams.ValidationMode = true;
		Vk::Vulkan::Init(VkParams);

	}

}
