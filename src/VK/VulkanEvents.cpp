#pragma once
#include "VulkanEvents.h"
#include "Devices/DeviceManager.h"
Landmark::Vk::Event_GpuTaskRequest::Event_GpuTaskRequest() :AvailableDevices(DeviceManager::GetDevices())
{
				
}
