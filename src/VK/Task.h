#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
#include "Devices/QueueFamily.h"

namespace Landmark {
	namespace Vk {

		class Queue;
		class Task
		{
			friend Queue;
		public:
			const std::string Name;
			enum TaskIntensities {
				INVALID = -1,
				VERY_HIGH = 100, //exclusive Queue
				HIGH = 70, //Queue with >lows
				MEDIUM = 30, //mix with >mediums
				LOW = 10, //self explanatory
				



			};
			const TaskIntensities taskType;


		private:
			Queue* Owner = nullptr;
			//TaskInfo _taskinfo;
			bool Initialized = false;
			VkCommandBuffer CmdBuffer;

			QueueFamily::QueueFamilyCapabilitiesMask Capabilities = 0;

			static inline Logger LOGGER = Logger("Task");

		protected:

			void Initiaize(Queue*);
		public:
			Task(std::string _Name,TaskIntensities _type);
			bool IsInitialized() { return Initialized; }
			void Begin();

			void End();



			void Upload();

			Queue* GetOwner() const { return Owner; }
		};
	}
}


