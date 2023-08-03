
#include "../src/Landmark.h"

#include <Containers/AllocVector.h>
#include "GameManager.h"

#ifdef _DEBUG
#define APPSTART int main(void)
#else
#include <Windows.h>
#define APPSTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif


void PrintAlloc(AllocVector_Medium<int, true>& alloc)
{
	std::cout << "== Printing ==\n";
	for (auto& v : alloc)
	{
		std::cout << v << "\n";
	}
}
APPSTART
{

	AllocVector_Medium<int,true> testAlloc {6,1,2,6,1,8,2,4};
	for (int i = 0 ; i < 10; i++)
	{
		testAlloc.Push(i);

	}
	PrintAlloc(testAlloc);
	for (int i = 0; i < 10; i+=2)
	{
		testAlloc.Erase(i);

	}
	PrintAlloc(testAlloc);
	for (int i = 25; i < 50; i += 2)
	{
		testAlloc.Push(i);

	}
	PrintAlloc(testAlloc);

	Landmark::Engine::AttachModule<GameManager>();
	Landmark::EngineInitParameters InitParameters;
	InitParameters.DebugMode = true;
	Landmark::Engine::Init(InitParameters);

}