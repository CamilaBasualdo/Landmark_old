#include "AudioSystem.h"


void Landmark::Audio::AudioSystem::ListDevices()
{
	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT")) {
		const ALCchar* devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
		const ALCchar* device = devices;
		std::vector<std::string>& _DeviceList = const_cast<std::vector<std::string>&>(AvailableSystemDevices);

		while (*device) {
			//printf("Device: %s\n", device);
			_DeviceList.push_back(device);
			device += strlen(device) + 1;
		}

			LOGGER.Log_List("Available Audio Devices:", AvailableSystemDevices);
	}
	else
		LOGGER.Error("This error means that the dev forgot to properly setup a fallback to ALC_ENUMERATE_ALL_EXT and you are fully wihtin your right to call her a dumbass");

			
}

void Landmark::Audio::AudioSystem::Init()
{
	LOGGER.Log("Init");

	


	ALCdevice* device = alcOpenDevice("");
	if (!device)
	{
		LOGGER.Error("Could not create OpenAL device.");
		
	}

	ALint attribs[6] = {
	0, 0
	};
	ALCcontext* context = alcCreateContext(device, attribs);
	if (!context)
	{
		LOGGER.Error("Could not create OpenAL context.");
		alcCloseDevice(device);
	
	}
	if (!alcMakeContextCurrent(context))
	{
		LOGGER.Error("Could not enable OpenAL context.");
		alcDestroyContext(context);
		alcCloseDevice(device);
		
	}

	auto eBufferFormat = alGetEnumValue("AL_FORMAT_51CHN16");
	if (eBufferFormat)
	{
		LOGGER.Log("Support for 5.1 channels");
	}

	auto Vendor = alGetString(AL_VENDOR);
	auto Renderer = alGetString(AL_RENDERER);
	if (Vendor)
		LOGGER.Log(std::string(Vendor));
	if (Renderer)
		LOGGER.Log(std::string(Renderer));
	ListDevices();
}

void Landmark::Audio::AudioSystem::Exit()
{
	LOGGER.Log("Shutdown");
}
