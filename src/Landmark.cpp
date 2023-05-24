#include "Landmark.h"

#include "Debug/Debug.h"
#include "Module/Module.h"
#include "Logic/RenderLogic.h"
namespace Landmark
{

	void Engine::Init(EngineInitParameters p)
	{
		



		

		//LOGGER.Log("Test Log", LOGGER.white, LOGGER.green);
		//LOGGER.Debug("Test Debug");
		//LOGGER.Warning("Test Warning");
		//LOGGER.Error("Test Error");
		//LOGGER.Critical("Test Critical\n");


		LOGGER.Debug("Engine Init!");
		Vk::VulkanInitParameters VkInitParams;
		VkInitParams.AppName = p.AppName;
		VkInitParams.ValidationMode = p.DebugMode;
		AttachModule<Vk::Vulkan>(VkInitParams);
		AttachModule<Vk::Vulkan>(VkInitParams );
		AttachModule<Render::RenderSystem>();

		LOGGER.Debug("Modules Init Stage 1!");
		for (auto M : _EngineModules)
			M.second->PreInit();

		LOGGER.Debug("Modules Init Stage 2!");
		for (auto M : _EngineModules)
			M.second->Init();


		LOGGER.Debug("Modules Init Stage 3!");
		for (auto M : _EngineModules)
			M.second->PostInit();

	}

}
