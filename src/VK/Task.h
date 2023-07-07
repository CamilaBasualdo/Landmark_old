#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
#include "Devices/QueueFamily.h"
namespace Landmark {
	namespace Vk {


		class Task
		{

		public:
			const std::string Name;
			enum TaskIntensities {
				INVALID = -1,
				VERY_HIGH = 100, //exclusive Queue
				HIGH = 70, //Queue with >lows
				MEDIUM = 30, //mix with >mediums
				LOW = 10,
				



			};
			const TaskIntensities taskType;


		private:
			//TaskInfo _taskinfo;
			bool Initialized = false;

			QueueFamily::QueueFamilyCapabilitiesMask Capabilities = 0;

		protected:

			void Initiaize();
		public:
			Task(std::string _Name,TaskIntensities _type);
			bool IsInitialized() { return Initialized; }
			void Begin();

			void End();



			void Upload();

		};
	}
}


