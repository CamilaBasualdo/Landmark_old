#pragma once
#include "../Events/EventSystem.h"
#include "../VK/VulkanEvents.h"
#include "../Module/Module.h"
#include "../Debug/Debug.h"
#include "../VK/Task.h"
namespace Landmark
{
	namespace Render
	{

		class RenderSystem :Module,Events::EventSubscriber
		{
			



			

			static inline Debug::Logger LOGGER = Debug::Debugger::GetLogger("RenderSystem");

			static inline Vk::Task* RenderingTask = nullptr;
		public:
		
			RenderSystem();

			// Inherited via IModule
			virtual std::string GetName() const override;

			virtual void PreInit() override;

			virtual void Init() override;

			virtual void PostInit() override;

		};
	}
}

