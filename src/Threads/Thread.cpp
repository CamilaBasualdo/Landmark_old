#include "Thread.h"


bool Landmark::Threads::Thread::GetShouldShutdown()
{ return CalledToShutdown; }

void Landmark::Threads::Thread::Join()
{
	_SystemThread.join();
}

bool Landmark::Threads::Thread::Joinable()
{
	return _SystemThread.joinable();
}
