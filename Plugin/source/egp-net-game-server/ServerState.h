#pragma once
#include <map>
#include <vector>
#include <chrono>

//class DemoState;
class DemoPeerManager;
class InputManager;

namespace RakNet
{
	struct Packet;
}

class ServerState
{
private:
	//DemoState* localState;

	InputManager* mpInputManager;

	bool runLoop;
	bool sendGameState;
	int updatesRecieved;

	//Timing
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> lastTimeMS;

public:
	ServerState();
	~ServerState();

	bool init();

	void updateState();

	inline bool shouldSendState() { return sendGameState; };
	inline bool shouldSendState(bool _flag) { return sendGameState = _flag; };
	void exitLoop();
	bool shouldLoop();
};

