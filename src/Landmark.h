#pragma once
#include "VK/Vulkan.h"

#include <map>
#include <typeinfo>
#include <typeindex>
#include "Module/Module.h"
#include "Logger.h"
namespace Landmark
{

	struct EngineInitParameters {
		bool IncludeDefaultModules = true;
		bool DebugMode = false;
		const char* AppName = "Default App Name";
		uint16_t UpdateRateHz = 60;

	};
	class Engine {

		static inline std::map<const std::type_info*, Module*> _EngineModules;
		static inline auto LOGGER = Logger("Landmark");

		static inline bool ShouldShutdown = false;
	
		static void DefaultModulesAttach();
		static void InitializationProcess();
		static void UpdateProcess();
		static void ShutdownProcess();
		static std::string GetExePath();
		static inline EngineInitParameters InitializationParameters;
#pragma region ASCIIlogos
		static inline const char* Logo32 = R"V0G0N(                    
                       /\                       
                       ||                       
                       ||                       
                       ||                       
                     _/'M\_                     
                   _/" ]I "\_                   
                 _/^   ](\_ `\\                 
              _//`     ]M/"\\__\\_              
            _//`       ]|    `"\__\_            
          _/^          ]|       `"\\\_          
        _/^         ___.|\__        "##_                        __              __       
       |^ _       //^` || `"\\       _\_|       |     /\  |\ | |  \  |\/|  /\  |__) |__/
       | |^\\_   /     ||    `\   _//^\||       |___ /~~\ | \| |__/  |  | /~~\ |  \ |  \
       | |   ^\\/`     ||      \//^   |||                   
       | |     /       ||       |     |||      
       | |     |       ||       |     |||                    Version: 0.0.1
       | |     |      ./M\      |,    |||       
       | |    ]'    _/']I`\____ ]I    |||       
       | |    |  _//"  ]I  ^\\_\_|    |||       
       | |    |//^     ]I     "\#(_   |||       
       | |__//^        ]I        `\\__|||       
       | ""`           ]I           `"^`|       
       |               ]I               |       
       |               ]I               |       
       |               ]I               |       
       |               ]I               |       
       |               ]I               |       
       |               ]I               |     
)V0G0N";

#pragma endregion
	public:
		template <class T, typename... Args>
		static typename std::enable_if<std::is_base_of<Landmark::Module, T>::value && !std::is_same<Landmark::Module, T>::value, void>::type
			AttachModule(Args&&... args) {
			//_EngineModules.contains
			const auto t = &typeid(T);
			if (_EngineModules.contains(&typeid(T))) {

				LOGGER.Error("Request to register module (" + std::string(typeid(T).name()).substr(6) + ") Failed. Module already exists!");
				return;
			}
			_EngineModules[&typeid(T)] = reinterpret_cast<Module*>(new T(std::forward<Args>(args)...));
			
		}

		static void Init(EngineInitParameters p);
		const static inline std::string ExePath = GetExePath();
		


		static void PrintLogo();
		static void Shutdown();
	};
}
