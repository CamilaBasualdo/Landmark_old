#pragma once
#include <Containers/AllocVector.h>
#include <Logger.h>

#include "ECS/Entity/Entity.h"

namespace Landmark::ECS
{
	//class Entity;
	//this is simply so entity has something to target


	class Component_Base
	{
		
		
	
	protected:
		
	};
	//T = derived Component
	template<typename T> class Component :protected Component_Base
	{
		friend Entity;

	private:

		


	public:
		Logger LOGGER = Logger("");
		
		Entity* const Owner = nullptr;

		

		



	protected:
		static AllocVector<T>::iterator Create(Entity* _Owner)
		{
			auto NewComp =  STORAGE.Push(T());
			auto& Comp = *NewComp;

			const_cast<Entity*&>(Comp.Owner) = _Owner;

			std::string N = typeid(T).name();
			auto posOfColon = N.find_last_of(':');
			if (posOfColon != std::string::npos)
			{
				N = N.substr(posOfColon);
			}
			else
			{
				N = N.substr(N.find_first_of(' ')+1);
			}
			Comp.LOGGER.ChangeName(_Owner->GetName() + "::" + N);
			//Comp.LOGGER = Logger();
			return NewComp;
		}

		static void Delete(AllocVector<T>::iterator where)
		{
			STORAGE.Erase(where.GetIndex());
		}

#ifdef _DEBUG
#define ERASE true
#else
#define ERASE false
#endif

		inline static AllocVector<T,size_t,ERASE> STORAGE;

		

	};
}
