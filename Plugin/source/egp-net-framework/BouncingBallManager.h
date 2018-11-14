#pragma once
#include <vector>
#include "BouncingBall.h"
#include <mutex>
#include "RakNet/NetworkIDManager.h"

class BouncingBallManager : public egpSerializableData
{
	
public:
	BouncingBallManager()
	{
		updateCoupledBalls = false;
		coupledBallUnits = new std::vector<std::vector<BouncingBall*>*>;
	}

	std::vector<BouncingBall*> ourBallUnits;
	std::vector<BouncingBall*> otherBallUnits;

	std::vector<std::vector<BouncingBall*>*>* coupledBallUnits; //i'm so sorry
	void update(float dt);

	BouncingBall* createBallUnit(Vector2 _position, Vector2 _velocity);
	
	// override serialize from base class
	virtual int Serialize(RakNet::BitStream *bs) const;

	// override deserialize from base class
	virtual int Deserialize(RakNet::BitStream *bs);
	int DeserializeOtherUnits(RakNet::BitStream *bs);
	void addCoupledBalls(RakNet::BitStream* _ballStream);
	std::vector<BouncingBall*>* combineGameStates();
	bool updateCoupledBalls;

	std::mutex ballLock;

	// aggregation
	// make your own cctor or assign op
	// cctor
	BouncingBallManager(const BouncingBallManager &) = delete;

	// assign
	BouncingBallManager &operator =(const BouncingBallManager &hrs) = delete;

	// mctor
	BouncingBallManager(BouncingBallManager &&) = delete;

	// move assign
	BouncingBallManager &operator =(BouncingBallManager &&) = delete;

	static BouncingBallManager* getInstance();
private:

};