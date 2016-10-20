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
	
	animation _sprite;
public:
	Actors();
	~Actors();

	const char* getSprite();
	int getX();
	int getY();

	void setPos(int x, int y);
	void setSprite(const char* name);
	int getClass();

	animation& getAnimation();
	void setState(const char* name);

	int combo;

};
#endif