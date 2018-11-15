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

	std::chrono::milliseconds elapsedChronoTime = currentTimeMS - lastTimeMS;
	std::chrono::milliseconds elapsedChronoNetworkTime = currentTimeMS - lastNetworkUpdateMS;

	float elapsedTime = (float)elapsedChronoTime.count();
	float elapsedNetworkTime = (float)elapsedChronoNetworkTime.count();

	if (elapsedNetworkTime > DemoPeerManager::getInstance()->networkTickRateMS)
	{
		//printf("Sending state to players\n");
		//broadcastState();

		lastNetworkUpdateMS = currentTimeMS;
	}

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
	lastNetworkUpdateMS = std::chrono::time_point_cast<std::chrono::milliseconds>(lastTime);

	return true;

}

ServerState* ServerState::getInstance()
{
	static ServerState instance;

	return &instance;
}