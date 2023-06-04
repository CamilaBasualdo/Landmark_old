#pragma once
#include "../Module/Module.h"
#include <al.h>
#include <alc.h>
#include "../Logger.h"
namespace Landmark::Audio
{
	class AudioSystem : Module
	{

		static inline const std::vector<std::string> AvailableSystemDevices = {};
		static inline Logger LOGGER = Logger("AudioSystem");



		static void ListDevices();

	public:
		std::string GetName() const override { return "AudioSystem"; }
		void Init() override;
		void Exit() override;
	};

}
