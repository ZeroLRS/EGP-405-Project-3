#pragma once

#include "Vector3.h"

struct Entity
{
	char* guid;
	int guidLength;

	Vector3 position;
	Vector3 velocity;
};