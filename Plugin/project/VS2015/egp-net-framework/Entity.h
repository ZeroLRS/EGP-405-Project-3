#pragma once

#include "Vector3.h"
#include <string>

class Entity
{
public:
	char* GUID;

	Vector3 position;
	Vector3 velocity;
};