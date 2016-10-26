#ifndef BOMB_H
#define BOMB_H

#include "Actors.h"
#include "Character.h"

#include <iostream>

using namespace std;

static const int DEF_BOMB_TIMER = 5;
static const int MAX_BOMB_TIMER = 5;

class bomb : public Actors
{
private:
	int _turns;
	int _fire;

	int _index;//index on the list of bombs
	int _team;//team of the owner of this bomb, -1 = nature forces of destruction
	int _owner;//owner id

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

	void init(int fire, int team = -1, int owner = -1);

	int getTeam();
	int getOwner();

	bomb* getBomb();
};
#endif