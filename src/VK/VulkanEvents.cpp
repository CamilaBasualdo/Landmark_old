#pragma once
#include "VulkanEvents.h"
#include "Devices/DeviceManager.h"
Landmark::Events::Event_GpuTaskRequest::Event_GpuTaskRequest() :AvailableDevices(Landmark::Vk::DeviceManager::GetDevices())
{
				
}
