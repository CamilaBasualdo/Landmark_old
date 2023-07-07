#include "Device.h"
#include "../../IO/WindowManager.h"

namespace Landmark
{
	namespace Vk
	{
		std::string Device::DeviceTypes_toString(DeviceTypes _type)
		{
			switch (_type)
			{
			case CPU: return "Cpu";
			case INTEGRATED: return "Integrated";
			case DISCRETE: return "Discrete";
			case VIRTUAL: return "Virtual";
			case OTHER: return "Other";
			default: ;
			}
		}


		Device::Device(PhysicalDevice* _PhysicalDevice) : _PhysicalDevice(_PhysicalDevice)
		{
		}
	}
}
