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

		ID_GAME_MESSAGE_1,
		UPDATE_NETWORK_PLAYER, // Client sends server a single entity
		UPDATE_GAME_STATE, // Server sends client update of all current entities

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
	std::string serverAddress;
	inline int getConnectedClients() { return mp_peer->NumberOfConnections(); };
	

	const unsigned int serverPort = 4040;
	// Time between ticks in MS - only used for sending
	const float networkTickRateMS = 100.0f;

	RakNet::RakPeerInterface* getPeer()
	{
		return mp_peer;
	}

	// Our functions called from the plugin
	void sendEntity(RakNet::BitStream* bs);
};


#endif	// !__EGPNET_SERVERPEERMANAGER_H_