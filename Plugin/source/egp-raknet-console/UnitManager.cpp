//#include "Header.h"
//
//typedef std::pair<int, Unit*> UnitPair;
//
//UnitManager::UnitManager()
//{
//}
//
//UnitManager::~UnitManager()
//{
//
//}
//
//void UnitManager::addUnit(GraphicsBuffer* graphic, float x, float y)
//{
//	Unit* newUnit = new Unit();
//	mUnitMap.insert(UnitPair(0, newUnit));
//}
//
//bool UnitManager::deleteUnit(Unit* _unit)
//{
//	for (auto iter = mUnitMap.begin(); iter != mUnitMap.end(); ++iter)
//	{
//		if ((*iter).second->mUnitID == _unit->mUnitID)
//		{
//			unitsToDelete.push(_unit->mUnitID);
//
//			return true;
//		}
//	}
//
//	return false;
//}
//
//void UnitManager::deleteUnitsInQueue()
//{
//	while (unitsToDelete.size() > 0)
//	{
//		int id = unitsToDelete.front();
//
//		delete mUnitMap.at(id);
//
//		mUnitMap.erase(id);
//	}
//}
//
//void UnitManager::update(float dt)
//{
//	for (auto iter = mUnitMap.begin(); iter != mUnitMap.end(); ++iter)
//	{
//		(*iter).second->update(dt);
//	}
//}
//
//Unit* UnitManager::getUnit(int _id)
//{
//	try
//	{
//		return mUnitMap.at(_id);;
//	}
//	catch(std::out_of_range)
//	{
//		return nullptr;
//	}
//}
//
//void UnitManager::draw(GraphicsSystem* mpGameSystem) // iterate through map to draw the units
//{
//	for (auto iter = mUnitMap.begin(); iter != mUnitMap.end(); ++iter)
//	{
//		(*iter).second->drawBitmap();
//	}
//}
//
//void UnitManager::clear()
//{
//	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
//	{
//		if (it->second != NULL)
//		{
//			delete it->second;
//		}
//	}
//
//	mUnitMap.clear();
//}