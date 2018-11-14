/*
	egpNetSerializableDataExample.h
	By Dan Buckstein
	(c) 2017-2018

	Example of a simple serializable data component.

	Additional contributions by (and date): 

*/

#ifndef __EGP_NET_SERIALIZABLEDATAEXAMPLE_H_
#define __EGP_NET_SERIALIZABLEDATAEXAMPLE_H_


#include "egpNetSerializableData.h"


// example
class egpSerializableDataExample : public egpSerializableData
{
	// RAW DATA (NO POINTERS, EXPLICIT OR INDIRECTLY): 
	//	store in a struct for maximum efficiency (see source)
	struct {
		int dummyInt;
		float dummyFlt;
		double dummyDbl;
	} data;

	// for ANYTHING that uses a pointer (e.g. dynamic array), 
	//	you must have some indicator of data size: 
	unsigned int dummyIntArraySz;
	unsigned int dummyIntArrayLen;
	int *dummyIntArray;

public: 

	// define ctors as needed; construct with required data if required
	// note: remember the dangers of storing pointers (e.g. dynamic arrays)
	egpSerializableDataExample(const int someInt = 0, const float someFlt = 0.0f, const double someDbl = 0.0, const unsigned int dynArrayLen = 1);

	// dtor as needed
	virtual ~egpSerializableDataExample();

	// override serialize from base class
	virtual int Serialize(RakNet::BitStream *bs) const;

	// override deserialize from base class
	virtual int Deserialize(RakNet::BitStream *bs);

	// aggregation
	// make your own cctor or assign op
	// cctor
	//egpSerializableDataExample(const egpSerializableDataExample &hrs) {};
	egpSerializableDataExample(const egpSerializableDataExample &) = delete;

	// assign
	//egpSerializableDataExample &operator =(const egpSerializableDataExample &hrs) { return *this; }
	egpSerializableDataExample &operator =(const egpSerializableDataExample &hrs) = delete;

	// mctor
	//egpSerializableDataExample(egpSerializableDataExample &&rhs) {};
	egpSerializableDataExample(egpSerializableDataExample &&) = delete;

	// move assign
	//egpSerializableDataExample &operator =(egpSerializableDataExample &&rhs) { return *this; }
	egpSerializableDataExample &operator =(egpSerializableDataExample &&) = delete;
};


#endif	// !__EGP_NET_SERIALIZABLEDATAEXAMPLE_H_