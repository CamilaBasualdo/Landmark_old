
#include "../src/Landmark.h"

#include <Containers/AllocVector.h>
#include "GameManager.h"

#ifdef _DEBUG
#define APPSTART int main(void)
#else
#include <Windows.h>
#define APPSTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif



APPSTART
{

	

	Landmark::Engine::AttachModule<GameManager>();
	Landmark::EngineInitParameters InitParameters;
	InitParameters.DebugMode = true;
	Landmark::Engine::Init(InitParameters);

}