#pragma once
#include "VulkanEvents.h"
#include "Devices/DeviceManager.h"
Landmark::Vk::Event_GpuTaskRequest::Event_GpuTaskRequest() :AvailableDevices([]()
{
		std::vector<Device> _D;
		auto _DMD = DeviceManager::GetDevices();
		for (auto entry : _DMD)
			_D.push_back(entry.second);
		return _D;
}())
{
				
}
