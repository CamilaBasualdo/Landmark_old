#pragma once
#include "../Events/EventSystem.h"
#include "../VK/VulkanEvents.h"
#include "../Module/Module.h"
#include "../Debug/Debug.h"
namespace Landmark
{
	namespace Render
	{

		class RenderSystem :Module,Events::EventSubscriber
		{
			



			void VulkanQueuesPreInitCallback(Vk::Event_VulkanQueuesPreInit& e) {

			}

			static inline Debug::Logger LOGGER = Debug::Debugger::GetLogger("RenderSystem");

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

