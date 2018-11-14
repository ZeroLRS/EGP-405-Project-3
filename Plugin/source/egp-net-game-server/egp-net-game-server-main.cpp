#include "egp-net-framework/DemoPeerManager.h"
#include "ServerState.h"
#include <iostream>


int main(int const argc, char const *const *const argv)
{
	ServerState* state = new ServerState();

	if (!state->init())
	{
		return -1;
	}

	while (1)
	{
		state->updateState();
	}

	return 0;
}