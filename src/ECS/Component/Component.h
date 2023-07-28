#pragma once
#include <Containers/lobby.h>
//T = derived Component
template<typename T> class Component
{


	
	

protected:
	static lobby<T>::address Create()
	{
		return STORAGE.Push(T());
	}

	static void Delete(lobby<T>::address where)
	{
		STORAGE.Erase(where.Location);
	}
	static lobby<T> STORAGE;

};