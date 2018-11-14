//#ifndef UNITMANAGER_H
//#define UNITMANAGER_H
//#include <map>
//#include <queue>
//
//class Unit;
//class GraphicsBuffer;
//class GraphicsSystem;
//
//class UnitManager
//{
//	std::map<int, Unit*> mUnitMap;
//	std::queue<int> unitsToDelete;
//public:
//	UnitManager();
//	~UnitManager();
//
//	void addUnit(GraphicsBuffer* _gBuffer, float x, float y);
//	void update(float dt);
//	int getNumUnits() { return mUnitMap.size(); }
//	void draw(GraphicsSystem* mpGameSystem);
//	void clear();
//
//	//TODO: change this to take in unit ID
//	bool deleteUnit(Unit* _unit);
//	void deleteUnitsInQueue();
//	Unit* getUnit(int _id);
//};
//
//#endif