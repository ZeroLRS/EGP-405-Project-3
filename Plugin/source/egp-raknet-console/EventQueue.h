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

#include "Event.h"
#include <mutex>

struct EventQueueItem
{
	Event* event;
	EventQueueItem* nextItem;
};

class EventQueue
{
public:
	EventQueue();
	~EventQueue();

	void push(Event* newEvent);
	Event* pop();

	int getSize();

	std::mutex queueMutex;

private:
	EventQueueItem* mFirstItem;
	EventQueueItem* mNewestItem;
	int mSize;
};
