#pragma once
#include "VK/Vulkan.h"
#include "Logic/RenderLogic.h"
#include <map>
#include <typeinfo>
#include <typeindex>
#include "Module/Module.h"
#include "Debug/Debug.h"
namespace Landmark
{
	class Render::RenderSystem;
	struct EngineInitParameters {
		bool DebugMode = false;
		const char* AppName = "Default App Name";

	};
	class Engine {

		static inline std::map<const std::type_info*, Module*> _EngineModules;
		static inline auto LOGGER = Debug::Debugger::GetLogger("Landmark");

	public:
		template <class T,typename... Args>
		static typename std::enable_if<std::is_base_of<Landmark::Module, T>::value && !std::is_same<Landmark::Module, T>::value, void>::type
			AttachModule(Args&&... args) {
			//_EngineModules.contains
			const auto t = &typeid(T);
			if (_EngineModules.contains(&typeid(T))) {

				LOGGER.Error("Request to register module (" + std::string(typeid(T).name()).substr(6) + ") Failed. Module already exists!");
				return;
			}
			_EngineModules[&typeid(T)] = reinterpret_cast<Module*>( new T(std::forward<Args>(args)...));
			LOGGER.Log("Registered Module (" + std::string(typeid(T).name()).substr(6) + ") ");
		}
		static void Init(EngineInitParameters p);
	};
}
