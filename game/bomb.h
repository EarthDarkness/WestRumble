#ifndef BOMB_H
#define BOMB_H

#include "Actors.h"
#include "Character.h"

#include <iostream>

using namespace std;

#define DEF_BOMB_TIMER 5
#define MAX_BOMB_TIMER 5

class Character;

class bomb : public Actors
{
private:
	int _turns;
	int _fire;

	Character* _character;

public:
	bomb();
	~bomb();

	void setFire(int fire);
	int getFire();

	void addTurn(int val);
	void setTurn(int val);
	int getTurn();
	
	bool checkTurnUp();
	bool checkTurnDown();

	void init(int fire, Character* c);
	void clear();

	//Actors* getActor();
	Character* getOwner();

};
#endif