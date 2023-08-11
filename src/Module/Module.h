#pragma once
#include <vector>
#include <string>
namespace Landmark {
	class Module {

	public:

		Module() {

		}

		

		
		virtual void PreInit(){};
		virtual void Init(){};
		virtual void PostInit() {};

		virtual void Update() {};

		virtual void PreExit() {};
		virtual void Exit() {};
		virtual void PostExit() {};
	};
}
