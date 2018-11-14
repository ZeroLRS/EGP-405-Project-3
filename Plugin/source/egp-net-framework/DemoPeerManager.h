/*
	egpNetServerPeerManager.h
	By Dan Buckstein
	(c) 2017-2018

	Specialized peer manager for server.

	Additional contributions by (and date): 

*/

#ifndef __EGPNET_SERVERPEERMANAGER_H_
#define __EGPNET_SERVERPEERMANAGER_H_

#include "egp-net/fw/egpNetPeerManager.h"
#include <vector>
class BouncingBall;

// server manager
class DemoPeerManager : public egpNetPeerManager
{
	// packet individual packet; function should not change manager
	//	packet: pointer to packet
	//	packetIndex: index in sequence of processed packets
	// return 0 to stop processing packets
	virtual int ProcessPacket(const RakNet::Packet *const packet, const unsigned int packetIndex) const;

public: 

	enum DemoPackets
	{
		e_id_packetBegin = ID_USER_PACKET_ENUM,
		e_id_gameStateUpdate,
		e_id_updateBouncingBalls,
		e_id_requestUpdateBouncingBallsToServer,
		e_id_coupledGameStateUpdate,
		e_id_spawnNewBall,
		e_id_sendStateCoupled,
		e_id_recieveCoupledState,
		// ****TO-DO: implement general identifiers
		// these should be peer-type-agnostic, i.e. either server or client
		// some suggestions: 
		//	e_id_stateInput,	// packet data contains remote input
		//	e_id_stateData,		// packet data contains info about game state
		// etc.

		// end; indicates where specialized managers can start
		e_id_packetEnd
	};

	// ctor
	DemoPeerManager();

	// dtor
	virtual ~DemoPeerManager();

	static DemoPeerManager* getInstance();
	void sendGameStatePacket(RakNet::BitStream* _gsStream, unsigned int _gsSize);
	void spawnNewBall(RakNet::BitStream* _bStream, unsigned int _bSize);
	void updateBouncingBalls(RakNet::BitStream* _ubStream, unsigned int _ubSize);
	void sendCoupledBouncingBalls(std::vector<BouncingBall*>* _balls);
	std::string serverAddress;
	inline int getConnectedClients() { return mp_peer->NumberOfConnections(); };

	int coupledPacketsRecieved;
};


#endif	// !__EGPNET_SERVERPEERMANAGER_H_