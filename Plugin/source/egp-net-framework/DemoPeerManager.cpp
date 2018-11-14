/*
	egpNetServerPeerManager.h
	By Dan Buckstein
	(c) 2017-2018

	Server manager source.

	Additional contributions by (and date): 

*/

#include "DemoPeerManager.h"
#include <iostream>
#include <mutex>

int DemoPeerManager::ProcessPacket(const RakNet::Packet *const packet, const unsigned int packetIndex) const
{
	// ****TO-DO: implement server-specific packet processor

	switch (packet->data[0])
	{
		// Leaving in sample cases for reference
		//case(e_id_gameStateUpdate):
		//{
		//	RakNet::BitStream stream(packet->data, packet->length, false);
		//
		//	stream.IgnoreBytes(sizeof((char)e_id_gameStateUpdate));
		//
		//	BouncingBallManager::getInstance()->Deserialize(&stream);
		//	BouncingBallManager::getInstance()->updateCoupledBalls = true;
		//
		//	std::cout << "gameStateUpdate" << std::endl;
		//	break;
		//}
		//case(e_id_requestUpdateBouncingBallsToServer):
		//{
		//	packet->data[0] = (char)e_id_updateBouncingBalls;
		//	RakNet::BitStream stream(packet->data, packet->length, false);
		//
		//	SendPacket(&stream, mp_peer->GetIndexFromSystemAddress(packet->systemAddress), true, true);
		//	std::cout << "requestUpdateBouncingBalls" << std::endl;
		//	break;
		//}
		case -100:
			break;
		default:
			std::cout << "ID" << packet->data[0] << std::endl;
			break;
	}

	return 0;
}


DemoPeerManager::DemoPeerManager()
{
	coupledPacketsRecieved = 0;
}

DemoPeerManager::~DemoPeerManager()
{
	// ****TO-DO
}

DemoPeerManager* DemoPeerManager::getInstance()
{
	static DemoPeerManager instance;

	return &instance;
}