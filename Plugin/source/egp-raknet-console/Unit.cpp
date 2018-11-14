//#include "Header.h"
//
//Unit::Unit()
//{
//	mpSprite = nullptr;
//}
//
//Unit::Unit(GraphicsBuffer* _spriteBuffer, Vector2 _position)
//{
//	mPosition = _position;
//	mpSprite = new Sprite(_spriteBuffer);
//}
//
//Unit::~Unit()
//{
//	cleanUp();
//}
//
//
//void Unit::cleanUp() 
//{
//	if (mpSprite != nullptr)
//	{
//		delete mpSprite;
//		mpSprite = nullptr;
//	}
//}
//Sprite* Unit::getSprite()
//{
//	return mpSprite;
//}
//
//void Unit::drawBitmap()
//{
//	//Game::getGraphicSystem()->draw(mpSprite, mPosition.x, mPosition.y);
//}
//
//void Unit::update(float dt)
//{}
