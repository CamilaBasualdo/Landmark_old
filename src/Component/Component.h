#pragma once

#include "../Containers/lobby.h"
#include "../Debug/Debug.h"
#include <typeinfo>
#include <string>
namespace Landmark {
	namespace ECS {

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
			static Debug::Logger LOGGER;

			// Inherited via _Base_Component

		};
	
	}
}

template<class T>
Landmark::Debug::Logger Landmark::ECS::Component<T>::LOGGER = Debug::Debugger::GetLogger(std::string(typeid(T).name()).substr(6));