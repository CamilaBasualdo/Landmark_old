#include "Landmark.h"


#include "Module/Module.h"
#include "Renderer/Renderer.h"
#include "IO/WindowManager.h"

#include "Debug/LogSystem.h"
#include "Time/Time.h"
#include "Audio/AudioSystem.h"
#include <thread>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#endif
namespace Landmark
{
	void Engine::DefaultModulesAttach()
	{
		//AttachModule<Threads::ThreadManager>();
		AttachModule<Vk::Vulkan>();
		AttachModule<Render::Renderer>();
		AttachModule<IO::WindowManager>();
		AttachModule<Time>();
		AttachModule<Audio::AudioSystem>();
	}

	void Engine::InitializationProcess()
	{
		LogSystem::Init();
		LogSystem::Log(std::string(Logo32) + "\n");


#ifdef _DEBUG
		//makes it look real cool
		std::cout << Logo32 << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
#endif

		LOGGER.Log(ExePath);
		if (InitializationParameters.IncludeDefaultModules) DefaultModulesAttach();

		LOGGER.Debug("Engine Init");


		LOGGER.Debug("Init Stage 1");
		for (auto M : _EngineModules)
			M.second->PreInit();

		LOGGER.Debug("Init Stage 2");
		for (auto M : _EngineModules)
			M.second->Init();


		LOGGER.Debug("Init Stage 3");
		for (auto M : _EngineModules)
			M.second->PostInit();
	}

	void Engine::UpdateProcess()
	{
		LOGGER.Log("Update Loop");
		while (!ShouldShutdown)
		{
			// Start measuring the loop execution time
			auto startTime = std::chrono::steady_clock::now();

			// Your loop code goes here
			// ...
			
			for (auto M : _EngineModules)
				M.second->Update();

			// Calculate the elapsed time for the loop iteration
			auto endTime = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

			static std::chrono::duration<double> loopDuration(1.0 / InitializationParameters.UpdateRateHz);

			// Sleep for the remaining time to achieve the desired frequency
			std::chrono::duration<double> sleepDuration = loopDuration - elapsedTime;
			if (sleepDuration > std::chrono::duration<double>(0))
				std::this_thread::sleep_for(sleepDuration);



		}
	}

	void Engine::ShutdownProcess()
	{
		LOGGER.Log("Shutting Down...");

		LOGGER.Log("Exit Stage 1");
		for (auto M : _EngineModules)
			M.second->PreExit();
		LOGGER.Log("Exit Stage 2");
		for (auto M : _EngineModules)
			M.second->Exit();
		LOGGER.Log("Exit Stage 3");
		for (auto M : _EngineModules)
			M.second->PostExit();


		LOGGER.Log("Goodbye!");

		LogSystem::Shutdown();
	}

	void Engine::Init(EngineInitParameters p) 
	{
		
		InitializationParameters = p;
		InitializationProcess();
		
		UpdateProcess();

		ShutdownProcess();

		
	}

	std::string Engine::GetExePath()
	{
		std::string path;
#ifdef _WIN32
		char buffer[MAX_PATH];
		GetModuleFileNameA(nullptr, buffer, MAX_PATH);
		path = buffer;
#elif defined(__linux__) || defined(__APPLE__)
		char buffer[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
		if (count != -1) {
			path = buffer;
		}
#endif
		return path;
	}


	void Engine::Shutdown()
	{
		if (!ShouldShutdown)
		{
			ShouldShutdown = true;
			LOGGER.Log("Shutdown Called");
		}
		
	}
}
