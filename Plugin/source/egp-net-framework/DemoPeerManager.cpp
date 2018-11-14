/*
	egpNetServerPeerManager.h
	By Dan Buckstein
	(c) 2017-2018

	Server manager source.

	Additional contributions by (and date): 

*/

#include "DemoPeerManager.h"
#include <iostream>
#include "BouncingBallManager.h"
#include <mutex>

int DemoPeerManager::ProcessPacket(const RakNet::Packet *const packet, const unsigned int packetIndex) const
{
	// ****TO-DO: implement server-specific packet processor

	switch (packet->data[0])
	{
		case(e_id_gameStateUpdate):
		{
			RakNet::BitStream stream(packet->data, packet->length, false);

			stream.IgnoreBytes(sizeof((char)e_id_gameStateUpdate));

			BouncingBallManager::getInstance()->Deserialize(&stream);
			BouncingBallManager::getInstance()->updateCoupledBalls = true;

			std::cout << "gameStateUpdate" << std::endl;
			break;
		}
		case(e_id_spawnNewBall):
		{
			RakNet::BitStream stream(packet->data, packet->length, false);

			stream.IgnoreBytes(sizeof((char)e_id_spawnNewBall));

			BouncingBall newBall;
			newBall.Deserialize(&stream);

			printf("New Ball Pos: %f, %f\nNew Ball Vel: %f, %f",
				newBall.position.x, newBall.position.y,
				newBall.velocity.x, newBall.velocity.y);

			BouncingBallManager::getInstance()->createBallUnit(newBall.position, newBall.velocity);
			break;
		}
		case(e_id_updateBouncingBalls):
		{
			RakNet::BitStream stream(packet->data, packet->length, false);

			stream.IgnoreBytes(sizeof((char)e_id_updateBouncingBalls));

			BouncingBallManager::getInstance()->DeserializeOtherUnits(&stream);
			std::cout << "UpdateBouncingBalls" << std::endl;

			
			break;
		}
		case(e_id_requestUpdateBouncingBallsToServer):
		{
			packet->data[0] = (char)e_id_updateBouncingBalls;
			RakNet::BitStream stream(packet->data, packet->length, false);

			SendPacket(&stream, mp_peer->GetIndexFromSystemAddress(packet->systemAddress), true, true);
			std::cout << "requestUpdateBouncingBalls" << std::endl;
			break;
		}
		case(e_id_sendStateCoupled): //on server
		{
			RakNet::BitStream stream(packet->data, packet->length, false);
			stream.IgnoreBytes(sizeof((char)e_id_sendStateCoupled));

			BouncingBallManager::getInstance()->addCoupledBalls(&stream);
			std::cout << "sendStateCoupled" << std::endl;
			break;
		}
		default:
			std::cout << "ID" << packet->data[0] << std::endl;
			break;
	}

	//if list of units is recieved send them to the unit manager in serverstate
	//--//
	//if inputs are recieved and current model is data push send input events to event system
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

void DemoPeerManager::sendGameStatePacket(RakNet::BitStream* _gsStream, unsigned int _gsSize)
{
	SendPacket(_gsStream, -1, true, true);
}

void DemoPeerManager::spawnNewBall(RakNet::BitStream * _bStream, unsigned int _bSize)
{
	SendPacket(_bStream, -1, true, true);
}

void DemoPeerManager::updateBouncingBalls(RakNet::BitStream * _ubStream, unsigned int _ubSize)
{
	SendPacket(_ubStream, 0, false, true);
}

void DemoPeerManager::sendCoupledBouncingBalls(std::vector<BouncingBall*>* _balls)
{
	RakNet::BitStream* bs = new RakNet::BitStream();

	sizeof((char)DemoPeerManager::e_id_gameStateUpdate);
	bs->Write((char)DemoPeerManager::e_id_gameStateUpdate);

	bs->Write(_balls->size());

	for (BouncingBall* b : *_balls)
	{
		b->Serialize(bs);
	}

	SendPacket(bs, -1, true, true);
}
