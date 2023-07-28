#pragma once

#include "../../Containers/lobby.h"
#include "../../Logger.h"
#include <typeinfo>
#include <string>

namespace Landmark {
	namespace ECS {
		class Entity;
		class ComponentBase {

			
		};
		template <class T> class Component : ComponentBase {

		private:
			friend class Entity;
			friend class Controller;
			class Controller  {
				friend Component;
			protected:
				static lobby<T>::address NewComponent() {
					auto add = _Instances.Push(T());
					return add;
				}

				static void DeleteComponent(lobby<ComponentBase>::address add) {
					_Instances.erase(add);

				}
				static inline lobby<T> _Instances = {};
			};
			

		private:
			lobby<T>::address ComponentAddress;
			lobby<Entity>::address OwnerAddress;
			
			
		protected:
			Component() {

					LOGGER.Debug("Constructed");

			}
			~Component() {
				LOGGER.Debug("Destroyed");
			}
			virtual void Init() { }

		public:
			static lobby<T>::address Instantiate(Entity* _owner) 
			{
				return Controller::NewComponent();
			}
			static void Destroy(lobby<T>::address _address);
			static inline Logger LOGGER = Logger(std::string(typeid(T).name()).substr(6));

			// Inherited via _Base_Component

		};
	
	}
}

//template<class T>
//Landmark::Debug::Logger Landmark::ECS::Component<T>::LOGGER = Debug::Debugger::GetLogger(std::string(typeid(T).name()).substr(6));