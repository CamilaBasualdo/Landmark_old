#pragma once
#include <functional>



#include "Event.h"
#include <map>

#include <vector>
#include <memory>
#include "../Logger.h"
#include <typeindex>
#include <typeinfo>

#define TemplateEventBaseCheck(RETURN) typename std::enable_if<std::is_base_of<Landmark::Events::Event, T>::value && !std::is_same<Landmark::Events::Event, T>::value, RETURN>::type
namespace Landmark
{
	namespace Events
	{
		class EventHandlerBase {
		public:
			virtual ~EventHandlerBase() {

			}

			virtual void Dispatch(Event*) = 0;
			virtual int GetSubscriberCount() = 0;
		};

		template <class T> class EventHandler : EventHandlerBase {
		public:
			using EventCallback = std::function<void(T&)>;

			void addSubscriber(EventCallback callback) {
				eventCallbacks.push_back(callback);
			}

			void Dispatch(Event* t) override {
				T& e = *reinterpret_cast<T*>(t);
				for (const auto& callback : eventCallbacks) {
					callback(e);
				}
			
			}

			virtual int GetSubscriberCount() override
			{
				return eventCallbacks.size();
			}


		private:
			std::vector<EventCallback> eventCallbacks;


			// Inherited via EventHandlerBase


		};
		class EventSystem
		{
			friend class EventDispatcher;
			friend class EventSubscriber;
			static inline std::map<const std::type_info*, EventHandlerBase*> EventHandlers = {};
			static inline Logger _EventSystem_Logger = Logger("EventSystem");

		protected:
			template <typename T>
			static TemplateEventBaseCheck(void) Dispatch(T& t) {

				std::string LogMessage = "Dispatching Event " + std::string(typeid(T).name()).substr(6) + ": ";

				if (!EventHandlers.contains(&typeid(T))) {
					LogMessage += "No Subscribers.";
					_EventSystem_Logger.Log(LogMessage);
					return;
				}
				else {
					LogMessage += std::to_string(EventHandlers[&typeid(T)]->GetSubscriberCount()) + " Subscriber(s).";
					_EventSystem_Logger.Log(LogMessage);
					//_EventSystem_Logger.Log(std::string("Dispatching Event ") + std::string(typeid(T).Name());
					EventHandlers[&typeid(T)]->Dispatch(reinterpret_cast<Event*>(&t));
				}


			}

			template <typename T>
			static TemplateEventBaseCheck(void)
				SubscribeTo(std::function<void(T&)> callback) {
				auto type = &typeid(T);
				EventHandler<T>* Handler;
				if (!EventHandlers.contains(type)) {
					Handler = new EventHandler<T>();
					EventHandlers[type] = reinterpret_cast<EventHandlerBase*>(Handler);
					/*
					EventHandlers.insert(
						std::pair<std::type_info*, std::unique_ptr<EventHandlerBase>>
						(type, std::make_unique< EventHandler<T>>() ));*/
				}
				else Handler = dynamic_cast<EventHandler<T>*>( EventHandlers[type]);
				
				Handler->addSubscriber(callback);
				_EventSystem_Logger.Log("New Subscription to " + std::string(type->name()).substr(6));
			}

		};

		class EventDispatcher {

		public:
			template <typename T, typename... Args>
			static TemplateEventBaseCheck(T)
				Dispatch(Args&&... args) {
				T _event = T(std::forward<Args>(args)...);
				EventSystem::Dispatch<T>(_event);
				return _event;
			}
			template <typename T>
			static TemplateEventBaseCheck(T)
				Dispatch() {
				T _event = T();
				 EventSystem::Dispatch<T>(_event);
				 return _event;
			}
		};


		class EventSubscriber {

			std::map<std::type_info, int> Subscriptions = {};
		public:
			EventSubscriber() {

			}
			~EventSubscriber() {
				EventSystem::_EventSystem_Logger.Log("Dont forget to implement unsubscribing!");
			}

			template <typename T>
			TemplateEventBaseCheck(void)
				SubscribeTo(std::function<void(T&)> callback) {
				//Subscriptions.insert(std::pair<std::type_info, int>(typeid(T), 0));
				EventSystem::SubscribeTo<T>(callback);
			}
			template <typename T>
			TemplateEventBaseCheck(void)
				UnsubscribeFrom() {
				EventSystem::_EventSystem_Logger.Log("Dont forget to implement unsubscribing!");
				//Subscriptions.insert(std::pair<std::type_info, int>(typeid(T), 0));
				//EventSystem::SubscribeTo<T>();
			}
		};
	}
}

