#pragma once
#include <vulkan/vulkan.h>
#include <string>
namespace Landmark {
	namespace Vk {

		
		class Task
		{
			
		public:
			struct TaskInfo {
				std::string Name;
				VkQueue queue;
				VkDevice device;
			};
			enum TaskTypes {
				INVALID = -1,
				CONTINUOUS = 0,
				BURST = 1,
				SLEEPER = 2
			};
			const TaskTypes taskType;

			enum Capabilities {
				GRAPHICS = VK_QUEUE_GRAPHICS_BIT,
				COMPUTE = VK_QUEUE_COMPUTE_BIT,
				TRANSFER = VK_QUEUE_TRANSFER_BIT,
				SPARSE_BINDING = VK_QUEUE_SPARSE_BINDING_BIT,
			};
		private:
			TaskInfo _taskinfo;
			bool Initialized = false;
			uint8_t CapabilitiesRequested = 0;
			uint8_t CapabilitiesAvailable = 0;

		protected:
			Task(TaskTypes _type);
			void Initiaize(TaskInfo _info);
		public:
			bool IsInitialized() { return Initialized; }
			void Begin();

			void End();



			void Upload();

		};
	}
}


