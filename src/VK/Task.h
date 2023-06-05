#pragma once
#include <vulkan/vulkan.hpp>
#include <string>
namespace Landmark {
	namespace Vk {


		class Task
		{

		public:
			
			enum TaskIntensities {
				INVALID = -1,
				VERY_HIGH = 100, //exclusive Queue
				HIGH = 70, //Queue with >lows
				MEDIUM = 30, //mix with >mediums
				LOW = 10,
				



			};
			const TaskIntensities taskType;

			using CapabilitiesMask = uint8_t;
			enum Capabilities {
				PRESENT = 0b1,
				GRAPHICS = 0b10,
				COMPUTE = 0b100,
				TRANSFER = 0b1000,
				SPARSE_BINDING = 0b10000
			};
		private:
			//TaskInfo _taskinfo;
			bool Initialized = false;

			CapabilitiesMask Capabilities = 0;

		protected:

			void Initiaize();
		public:
			Task(TaskIntensities _type);
			bool IsInitialized() { return Initialized; }
			void Begin();

			void End();



			void Upload();

		};
	}
}


