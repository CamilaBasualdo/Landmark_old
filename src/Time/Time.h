#pragma once
#include "../Logger.h"
#include "../Module/Module.h"
#include <chrono>
namespace Landmark
{
	class Time:Module
	{
		static inline Logger LOGGER = Logger("Time");
		static inline std::chrono::steady_clock::time_point TimeOfStart = std::chrono::steady_clock::now();
	public:
		std::string GetName() const override { return "Time"; }
		void Init() override;
	};
}