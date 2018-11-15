#include "ServerState.h"
#include <iostream>
#include <string>
#include "egp-net-framework/DemoPeerManager.h"

ServerState::ServerState()
{
	
}


ServerState::~ServerState()
{
	
}

void ServerState::updateState()
{

	std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> currentTimeMS
		= std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);

	std::chrono::microseconds elapsedChronoTime = currentTimeMS - lastTimeMS;

	float elapsedTime = (float)elapsedChronoTime.count();

	//printf("elapsedTime: %f", elapsedTime);

	//mpBouncingBallManager->update(elapsedTime / 1000);

	lastTimeMS = currentTimeMS;

}

void ServerState::exitLoop()
{
	runLoop = false;
}

bool ServerState::shouldLoop()
{
	return runLoop;
}

bool ServerState::init()
{

	int numClients;
	int port = DemoPeerManager::getInstance()->serverPort;
	std::cout << "Enter maximum number of clients: \n";

	std::cin >> numClients;

	if (!DemoPeerManager::getInstance()->StartupNetworking(false, numClients, port, true))
	{
		return false;
	}

	//Timing
	lastTime = std::chrono::system_clock::now();
	lastTimeMS = std::chrono::time_point_cast<std::chrono::milliseconds>(lastTime);

	return true;

}
