#include "ServerState.h"
#include <iostream>
#include <string>
#include "egp-net-framework/BouncingBallManager.h"
#include "egp-net-framework/DemoPeerManager.h"

ServerState::ServerState()
{
	
}


ServerState::~ServerState()
{
	
}

std::string getModelAsString(DataModel _model)
{
	switch (_model)
	{
	case(OFFLINE):
		return "Offline";
	case(PUSH):
		return "Push";
	case(SHARE):
		return "Shared";
	case(COUPLED):
		return "Coupled";
	default:
		return "";
	}
}

void ServerState::updateState()
{

	switch (getCurrentModel())
	{
		case(PUSH):
		{
			updateDataPush();
			break;
		}
		case(SHARE):
		{
			updateDataShared();
			break;
		}
		case(COUPLED):
		{
			updateDataCoupled();
			break;
		}
		default:
		{
			return;
		}
	}

}

void ServerState::simulateDemo()
{
	std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> currentTimeMS
		= std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);

	std::chrono::microseconds elapsedChronoTime = currentTimeMS - lastTimeMS;

	float elapsedTime = (float)elapsedChronoTime.count();

	mpBouncingBallManager->update(elapsedTime / 1000);

	lastTimeMS = currentTimeMS;
	std::cout << mpBouncingBallManager->ourBallUnits.size() << std::endl;
}

void ServerState::updateDataPush()
{
	//update regularly
	simulateDemo();


	std::lock_guard<std::mutex> lock(mpBouncingBallManager->ballLock);
	RakNet::BitStream* bs = new RakNet::BitStream();
	unsigned int packetSize = 0;
	packetSize += sizeof((char)DemoPeerManager::e_id_gameStateUpdate);
	//printf("PacketSize: %i", packetSize);
	bs->Write((char)DemoPeerManager::e_id_gameStateUpdate);
	packetSize += mpBouncingBallManager->Serialize(bs);
	DemoPeerManager::getInstance()->sendGameStatePacket(bs, packetSize);

	
}

void ServerState::updateDataShared()
{
	//recieve networking events
	//immidiately broadcast them to all clients
}

void ServerState::updateDataCoupled()
{
	//wait to recieve all messages from clients (has counter of num connected)
	//when all are recieved set positions and broadcast to all clients
	//if there are conflicting positions average them

	if (shouldSendState())
	{
		//send state to all clients
		//server->broadcastDemoState();

		//prevent state from being sent again until all packets are recieved
		updatesRecieved = 0;
		shouldSendState(false);
	}
}

void ServerState::exitLoop()
{
	runLoop = false;
}

bool ServerState::shouldLoop()
{
	return runLoop;
}

bool ServerState::initPush()
{

	return true;
}

bool ServerState::initShare()
{
	return true;
}

bool ServerState::init()
{
	mpBouncingBallManager = BouncingBallManager::getInstance();;

	std::string modelSelect;
	std::cout << "Select Model:\n\tData (P)ush\n\tData (S)hare\n\tData (C)oupled\n";
	std::cin >> modelSelect;
	if (modelSelect[0] == 'p' || modelSelect[0] == 'P')
	{
		currentDataModel = PUSH;
		initPush();
	}
	else if (modelSelect[0] == 's' || modelSelect[0] == 'S')
	{
		currentDataModel = SHARE;
		initShare();
	}
	else if (modelSelect[0] == 'c' || modelSelect[0] == 'C')
	{
		currentDataModel = COUPLED;
	}
	else
	{
		std::cout << "\n\nERROR, invalid input.\n";
		system("pause");
		return false;
	}

	int numClients;
	int port;
	std::cout << "Enter maximum number of clients: \n";

	std::cin >> numClients;

	std::cout << "Enter server port: \n";
	std::cin >> port;

	if (!DemoPeerManager::getInstance()->StartupNetworking(false, numClients, port, true))
	{
		return false;
	}

	//Timing
	lastTime = std::chrono::system_clock::now();
	lastTimeMS = std::chrono::time_point_cast<std::chrono::milliseconds>(lastTime);

	return true;

}
