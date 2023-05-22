#pragma once
#include <vector>


class IModule {


public:



	IModule() {
		
	}

	// Inherited via ModuleBase
	virtual std::string GetName() const = 0;

	virtual void PreInit() = 0;
	virtual void Init() = 0;
	virtual void PostInit() = 0;
};