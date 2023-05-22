#pragma once
#include "../Component/Component.h"
#include <map>
#include <typeinfo>
namespace Landmark {
	namespace ECS {
		class Entity {


			std::map<std::string, lobby<ComponentBase>::address> Components = {};


		public:
			template<typename T, typename std::enable_if<std::is_base_of<ComponentBase, T>::value>::type* = nullptr>
			lobby<T>::address GetComponent() {

				for (auto entry : Components) {
					if (entry.first == typeid(T).name())
						return entry.second;
				}

				return lobby<T>::address();
			}
			template<typename T, typename std::enable_if<std::is_base_of<ComponentBase, T>::value>::type* = nullptr>
			lobby<T>::address AddComponent() {
				auto newCompAddress = Component<T>::Instantiate(this);
				lobby< ComponentBase>::address casted(newCompAddress);
				Components.emplace(std::pair<std::string, lobby<ComponentBase>::address>(typeid(T).name(), casted));
				//Components.(std::pair<std::string,lobby<_Base_Component>>(typeid(T).name(),casted));
				return newCompAddress;
				
			}

			
		};
	}
}
