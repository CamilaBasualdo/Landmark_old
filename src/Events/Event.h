#pragma once
#include "Event.h"
namespace Landmark
{
	namespace Events
	{
		class Event
		{
		};


		class CancellableEvent : public Event
		{
			bool IsCancelled = false;
		public:
			void SetIsCancelled(bool state) { IsCancelled = state; }
			bool GetIsCancelled() { return IsCancelled; }
		};

	}
}
