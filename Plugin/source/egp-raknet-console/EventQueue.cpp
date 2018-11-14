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

#include "EventQueue.h"

EventQueue::EventQueue()
{
	mFirstItem = nullptr;
	mNewestItem = nullptr;
	mSize = 0;
}

EventQueue::~EventQueue()
{
}

int EventQueue::getSize()
{
	return mSize;
}

void EventQueue::push(Event* newEvent)
{
	// Create the new item
	EventQueueItem* newItem = new EventQueueItem();
	newItem->event = newEvent;
	newItem->nextItem = nullptr;

	// Connect it to the rest of the queue
	if (mFirstItem != nullptr)
	{
		mNewestItem->nextItem = newItem;
	}
	else // If there were no items in the queue, set this one to the first
	{
		mFirstItem = newItem;
	}
	mNewestItem = newItem;

	++mSize;
}

Event* EventQueue::pop()
{
	if (mFirstItem != nullptr)
	{
		// Get a pointer to the current top and iterate to the next item
		EventQueueItem* currentItem = mFirstItem;
		mFirstItem = currentItem->nextItem;

		// Get the event we need and then delete the current item
		Event* returnEvent = currentItem->event;
		delete(currentItem);

		--mSize;

		return returnEvent;
	}
	else // If there's nothring in the queue, return nullptr
	{
		return nullptr;
	}
}
