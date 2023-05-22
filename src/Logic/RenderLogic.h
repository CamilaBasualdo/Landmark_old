#pragma once
#include "../Events/EventSystem.h"
#include "../VK/VulkanEvents.h"
#include "../Module/Module.h"
namespace Landmark
{
	namespace Render
	{

		class RenderLogic : IModule,Events::EventSubscriber
		{
			RenderLogic();



			void VulkanQueuesPreInitCallback(Vk::Event_VulkanQueuesPreInit& e) {

			}
		public:
		
			

			// Inherited via IModule
			virtual std::string GetName() const override;

			virtual void PreInit() override;

			virtual void Init() override;

			virtual void PostInit() override;

		};
	}
}

