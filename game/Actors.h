#ifndef ACTORS_H
#define ACTORS_H

#include <string>
#include "../engine/animation.h"

using namespace std;

enum ACTOR_CLASS{
	ACTOR_ACTOR=0,
	ACTOR_CHAR,
	ACTOR_POWUP,
	ACTOR_BOMB,
	ACTOR_BLOCK
};

class block;
class bomb;
class Character;
class PowerUp;

class Actors
{
protected:
	int _class;
	int _x,_y;

	bool _index;

	int _anim;	
	animation _sprite;
public:
	Actors();
	~Actors();

	int getSprite();
	int getX();
	int getY();

	void setPos(int x, int y);
	int getClass();

	void setAnimation(animation* table[],int id,int stt=0);
	animation& getAnimation();

	void setState(int stt);

	void activate(int index);
	void deactivate();
	int getIndex();
	bool isActive();

	virtual block* getBlock();
	virtual bomb* getBomb();
	virtual Character* getCharacter();
	virtual PowerUp* getPowerUp();
	

};
#endif