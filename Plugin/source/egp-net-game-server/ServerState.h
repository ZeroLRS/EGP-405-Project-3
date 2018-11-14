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

enum DataModel
{
	PUSH = 0,
	SHARE,
	COUPLED,
	OFFLINE
};

class ServerState
{
private:
	//DemoState* localState;

	InputManager* mpInputManager;

	DataModel currentDataModel;
	bool runLoop;
	bool sendGameState;
	int updatesRecieved;

	void simulateDemo();
	void updateDataPush();
	void updateDataShared();
	void updateDataCoupled();

	bool initPush();
	bool initShare();
	//bool initCoupled();

	//Timing
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> lastTimeMS;

public:
	ServerState();
	~ServerState();

	bool init();

	void updateState();

	inline DataModel getCurrentModel() { return currentDataModel; };
	inline DataModel setCurrentModel(DataModel _nextModel) { return currentDataModel = _nextModel; };
	inline bool shouldSendState() { return sendGameState; };
	inline bool shouldSendState(bool _flag) { return sendGameState = _flag; };
	void exitLoop();
	bool shouldLoop();
};

