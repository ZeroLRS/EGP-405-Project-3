/* File created by:
* Lucas Spiker and Cory Smith
* For:
* Lab 3 on 10/1/2018
* EGP-405 by Dan Buckstein
*
* “We certify that this work is entirely our own.
*	The assessor of this project may reproduce
*	this project and provide copies to other academic staff,
*	and/or communicate a copy of this project to a plagiarism-checking service,
*	which may retain a copy of the project on its database.”
*/

#pragma once
#include <map>
#include "Event.h"

class EventQueue;
class EventListener;

class EventSystem
{
public:
	EventSystem();
	~EventSystem();

	static EventSystem* getInstance();

	void addListener(EventListener* _listener, EventType _type);
	void removeListenerFromEvent(EventListener* _pListener, EventType _type);
	void removeListenerFromAllEvents(EventListener* _pListener);

	void addToEventQueue(Event* _event);
	//void addToEventQueue(Event* _event, bool isServer = false);
	void dispatchEvents();

private:

	EventQueue* eventQueue;
	std::multimap<EventType, EventListener*> mListenerMap;

	void dispatchEvent(const Event& _event);
};

