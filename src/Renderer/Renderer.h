#pragma once
#include "../Events/EventSystem.h"
#include "../VK/VulkanEvents.h"
#include "../Module/Module.h"
#include "../logger.h"
#include "../VK/Task.h"
#include <thread>
#include <VK/GraphicsPipeline.h>
namespace Landmark::IO
{
	class Window;
}
namespace Landmark::Render
{
	class Event_RendererInit : Events::Event {
	};
	class Renderer :Module, Events::EventSubscriber,Events::EventDispatcher
	{

		static inline Logger LOGGER = Logger("Renderer");

		static inline Vk::Task* RenderingTask = nullptr;
		
		static inline std::thread RenderingThread;

		static inline bool CloseThread = false;

		static inline VkRenderPass RenderPass;
		static inline VkSemaphore RenderFinishedSemaphore;
		static inline VkFence inFlightFence; //to make sure only one frame is rendered at a time

		static inline Vk::GraphicsPipeline DefaultGBufferShader;
	public:

		Renderer();

		
		

		virtual void PreInit() override;

		virtual void Init() override;

		virtual void PostInit() override;


		static VkRenderPass GetRenderPass() { return RenderPass; }

	private:
		static void RenderingThreadStart();
		static void InitRenderPass();
		static void CreateDefaultGBufferShader();


		static void Render();

	public:
		void PreExit() override;
	};
}


