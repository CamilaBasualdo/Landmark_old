#pragma once
#include <map>
#include <Containers/AllocVector.h>
#include <typeinfo>
#include <Logger.h>
//#include "ECS/Component/Component.h"
#define ENTITY_TEMPLATEPROLOGUE template<typename T, typename std::enable_if<std::is_base_of<Component_Base, T>::value>::type* = nullptr>
namespace Landmark::ECS
{
	class Component_Base;
	class Entity
	{
		
		using Type_Name = const char*;
		//key is typeid(T).name();
		std::map<Type_Name, AllocVector<Component_Base>::iterator> Components;

		const std::string Name;

		static inline Logger LOGGER = Logger("Entity");

	public:
		Entity(std::string _Name) :Name(_Name) {};
		std::string GetName() const { return Name; }



		ENTITY_TEMPLATEPROLOGUE
		AllocVector<T>::iterator AddComponent()
		{
			auto CompName = typeid(T).name();
			if (Components.contains(CompName))
			{
				LOGGER.Warning(Name + " > AddComponent Failed! Already has component of type: " + std::string(CompName));
				return AllocVector<T>::iterator(nullptr, -1);
			}

			auto newComp = T::Create(this);
			Components.insert(std::pair<Type_Name, AllocVector<Component_Base>::iterator>(CompName, AllocVector<Component_Base>::iterator(newComp)));
			LOGGER.Log(Name + " Attached Component (" + CompName + ")");
			return newComp;
		}

		ENTITY_TEMPLATEPROLOGUE
		AllocVector<T>::iterator GetComponent(bool* FoundQuery = nullptr)
		{
			auto CompName = typeid(T).name();

			if (Components.contains(CompName))
			{
				if (FoundQuery)
					*FoundQuery = true;
				auto it = Components.at(CompName);
				return AllocVector<T>::iterator(it);
			}
			else
			{
				if (FoundQuery)
					*FoundQuery = false;
				return AllocVector<T>::iterator(nullptr, -1);
			}
			
		}
		ENTITY_TEMPLATEPROLOGUE
		bool HasComponent()
		{
			return Components.contains(typeid(T).name());
		}
		
		ENTITY_TEMPLATEPROLOGUE
		void RemoveComponent()
		{
			auto CompName = typeid(T).name();
			if (Components.contains(CompName))
			{
				auto& Comp = Components[CompName];
				//auto Comp = AllocVector<T>::iterator(nullptr, -1);
				T::Delete(AllocVector<T>::iterator(Comp));
				//T::Delete(AllocVector<T>::iterator(nullptr,-1));
				Components.erase(CompName);
				
			}
			else
			{
				LOGGER.Warning(Name + " > RemoveComponent Failed! Does not have component of type: " + std::string(CompName));
			}
		}
		

	};
}
