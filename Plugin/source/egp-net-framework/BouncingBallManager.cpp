#include "BouncingBallManager.h"
#include <map>
#include "DemoPeerManager.h"
void BouncingBallManager::update(float dt)
{
	std::lock_guard<std::mutex> lock(ballLock);
	for each (BouncingBall* ball in ourBallUnits)
	{
		ball->update(this, dt);
	}
}

BouncingBall * BouncingBallManager::createBallUnit(Vector2 _position, Vector2 _velocity)
{
	std::lock_guard<std::mutex> lock(ballLock);
	BouncingBall* newUnit = new BouncingBall();
	newUnit = new BouncingBall();
	newUnit->position = _position;
	newUnit->velocity = _velocity;
	newUnit->netID = RakNet::RakPeerInterface::Get64BitUniqueRandomNumber();

	ourBallUnits.push_back(newUnit);

	return newUnit;
}

int BouncingBallManager::Serialize(RakNet::BitStream * bs) const
{
	if (bs)
	{
		unsigned int totalSz = 0;

		totalSz += sizeof(ourBallUnits.size());
		bs->Write(ourBallUnits.size());

		for (BouncingBall* b : ourBallUnits)
		{

			totalSz += b->Serialize(bs);
		}

		return totalSz;
	}
	return 0;
}

int BouncingBallManager::Deserialize(RakNet::BitStream * bs)
{
	std::lock_guard<std::mutex> lock(ballLock);
	if (bs)
	{
		//clean up old ballunits
		//ourBallUnits.clear();

		unsigned int totalSz = 0;
		size_t ballCount = 0;

		totalSz += sizeof(ballCount);
		bs->Read(ballCount);

		for (int i = 0; i < ballCount; i++)
		{
			BouncingBall* newBall = new BouncingBall();
			newBall = new BouncingBall();

			totalSz += newBall->Deserialize(bs);

			bool found = false;
			for (BouncingBall* currentBall : ourBallUnits)
			{
				if (currentBall->netID == newBall->netID)
					found = true;
			}

			if (!found)
			{
				ourBallUnits.push_back(newBall);
			}
		}

		return totalSz;
	}
	return 0;
}

int BouncingBallManager::DeserializeOtherUnits(RakNet::BitStream * bs)
{
	std::lock_guard<std::mutex> lock(ballLock);
	if (bs)
	{

		unsigned int totalSz = 0;
		size_t ballCount = 0;

		totalSz += sizeof(ballCount);
		bs->Read(ballCount);

		for (int i = 0; i < ballCount; i++)
		{
			BouncingBall* newBall = new BouncingBall();
			newBall = new BouncingBall();

			totalSz += newBall->Deserialize(bs);

			bool found = false;
			for (BouncingBall* currentBall : otherBallUnits)
			{
				if (currentBall->netID == newBall->netID)
					found = true;
			}

			if (!found)
			{
				otherBallUnits.push_back(newBall);
			}
		}

		return totalSz;
	}
	return 0;
}

void BouncingBallManager::addCoupledBalls(RakNet::BitStream* _ballStream)
{
	std::vector<BouncingBall*>* toAdd = new std::vector<BouncingBall*>();

	size_t ballCount = 0;
	_ballStream->Read(ballCount);

	for (int i = 0; i < ballCount; i++)
	{
		BouncingBall* newBall = new BouncingBall();
		newBall = new BouncingBall();

		newBall->Deserialize(_ballStream);
		
		toAdd->push_back(newBall);
	}

	coupledBallUnits->push_back(toAdd);

	(DemoPeerManager::getInstance()->coupledPacketsRecieved)++;

	if (DemoPeerManager::getInstance()->coupledPacketsRecieved >= DemoPeerManager::getInstance()->getConnectedClients())
	{
		combineGameStates();
	}
}

bool containsBall(std::map<int64_t, BouncingBall*>* _map, uint64_t _id)
{
	auto iter = _map->find(_id);

	return iter != _map->end();
}

std::vector<BouncingBall*>* BouncingBallManager::combineGameStates()
{
	typedef std::pair<int64_t, BouncingBall*> coupledUnitData;

	std::map<int64_t, BouncingBall*> combined;

	int numClients = DemoPeerManager::getInstance()->getConnectedClients();

	for (auto iter = coupledBallUnits->begin(); iter != coupledBallUnits->end(); ++iter)
	{
		for (auto subIter = (*iter)->begin(); subIter != (*iter)->end(); ++subIter)
		{
			if (containsBall(&combined, (*subIter)->netID))
			{
				combined.at((*subIter)->netID)->position += (*subIter)->position;
			}
			else
			{
				combined.insert(coupledUnitData((*subIter)->netID, (*subIter)));
			}
		}
	}

	std::vector<BouncingBall*>* combinedBalls = new std::vector<BouncingBall*>();

	for (auto iter = combined.begin(); iter != combined.end(); ++iter)
	{
		float x = iter->second->position.x;
		float y = iter->second->position.y;

		x /= numClients;
		y /= numClients;

		iter->second->position.x = x;
		iter->second->position.y = y;

		combinedBalls->push_back((iter->second));
	}

	DemoPeerManager::getInstance()->sendCoupledBouncingBalls(combinedBalls);

	DemoPeerManager::getInstance()->coupledPacketsRecieved = 0;
	coupledBallUnits->clear();
	ourBallUnits.clear();

	return combinedBalls;
}

BouncingBallManager* BouncingBallManager::getInstance()
{
	static BouncingBallManager instance;

	return &instance;
}
