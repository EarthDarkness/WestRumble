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

class Actors
{
protected:
	int _class;
	int _x,_y;

	int _anim;
	
	animation _sprite;
public:
	Actors();
	~Actors();

	int getSprite();
	int getX();
	int getY();

	void setPos(int x, int y);
	//void setSprite(const char* name);
	int getClass();

	void setAnimation(animation* table[],int id,int stt=0);
	animation& getAnimation();

	void setState(int stt);

	int combo;

};
#endif