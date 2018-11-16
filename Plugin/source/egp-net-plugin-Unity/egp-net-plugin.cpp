/*
	EGP Networking: Plugin Interface/Wrapper
	Dan Buckstein
	October 2018

	Main implementation of Unity plugin wrapper.
*/

#include "egp-net-plugin.h"

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"

#include "egp-net-framework/DemoPeerManager.h"
#include "egp-net-framework/Vector3.h"

#pragma pack(push, 1)

struct GameMessageData
{
	unsigned char typeID;

	// ****TO-DO: implement game message data struct

};

#pragma pack (pop)

// create and return instance of peer interface
RakNet::RakPeerInterface *peer;

DemoPeerManager* peerManager;

extern "C"
{
	// dummy function for testing
	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	int foo(int bar)
	{
		return (bar + 1);
	}

	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	bool initNetworking(char* ip)
	{
		peerManager = DemoPeerManager::getInstance();

		if (peerManager->StartupNetworking(true, 0, 0, false) == 0)
		{
			return false;
		}

		peerManager->serverAddress = ip;
		if (peerManager->Connect(ip, peerManager->serverPort) <= 0)
		{
			return false;
		}

		peer = peerManager->getPeer();

		return true;

	}

	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	char* handlePacket(int* length)
	{
		RakNet::Packet* packet;

		packet = peer->Receive();
		if (!packet)
		{
			*length = sizeof("nopacket");
			return "nopacket";
		}
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			printf("Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("Another client has connected.\n");
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			printf("Our connection request has been accepted.\n");
			{
				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, 
				//	and handle endian swapping automatically
		
				//RakNet::BitStream bsOut;
				//bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				//bsOut.Write("Hello world");
				//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				
				// ****TO-DO: write and send packet without using bitstream
			}
			break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			//if (isServer) {
				//printf("A client has disconnected.\n");
			//}
			//else {
				printf("We have been disconnected.\n");
			//}
			break;
		case ID_CONNECTION_LOST:
			//if (isServer) {
			//	printf("A client lost the connection.\n");
			//}
			//else {
				printf("Connection lost.\n");
			//}
			//break;
		
		case DemoPeerManager::ID_GAME_MESSAGE_1:
			printf("DEBUG MESSAGE: received packet ID_GAME_MESSAGE_1.\n");
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				//printf("%s\n", rs.C_String());
		
				char* returnThis = (char*)rs.C_String();
		
				*length = (int) strlen(returnThis);
				return returnThis;
		
				// ****TO-DO: read packet without using bitstream
		
			}
			break;
		
		case DemoPeerManager::UPDATE_NETWORK_PLAYER:
			printf("DEBUG MESSAGE: received packet ID_GAME_MESSAGE_1.\n");
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::Time dt, ourt, theirt;

				ourt = RakNet::GetTime();
				bsIn.Read(theirt);
				dt = ourt - theirt;

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)DemoPeerManager::UPDATE_NETWORK_PLAYER);
				bsOut.Write(sizeof(uint64_t));
				bsOut.Write(dt);
				bsOut.Write(bsIn);

				bsOut.Read(rs);

				char* returnThis = (char*)rs.C_String();

				*length = (int)strlen(returnThis);
				return returnThis;
			}
			break;
		
		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}

		peer->DeallocatePacket(packet);
		char tmpStr[] = "nodata.";
		tmpStr[7] = packet->data[0];
		char* toSend = (char*)malloc(strlen(tmpStr) + 1);
		strcpy(toSend, tmpStr);
		*length = sizeof("nodata.");
		return toSend;
	}

	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	char* plsreturn(int* length)
	{
		char* arrayTest = "yeeting";
		*length = sizeof("yeeting");
		return arrayTest;
	}

	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	bool sendEntityToServer(int guidLength, char* guid, Vector3 position, Vector3 destination)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)DemoPeerManager::UPDATE_NETWORK_PLAYER);

		const RakNet::Time timeSend = RakNet::GetTime();
		bsOut.Write(timeSend);

		bsOut.Write(guidLength);

		for (int i = 0; i < guidLength; i++)
		{
			bsOut.Write(guid[i]);
		}

		bsOut.Write(position);
		bsOut.Write(destination);

		peerManager->sendEntity(&bsOut);

		//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
		//	RakNet::SystemAddress(peerManager->serverAddress.c_str()), false);

		return true;
	}

	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	bool disconnect()
	{
		if (peerManager->Disconnect() != 0)
			return true;

		return false;

	}

	__declspec(dllexport)	// tmp linker flag, forces lib to exist
	bool shutdownNetworking()
	{
		if (peerManager->ShutdownNetworking() != 0)
			return true;

		return false;

	}
}
