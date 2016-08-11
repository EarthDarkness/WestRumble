#ifndef CHARACTER_H
#define CHARACTER_H

#include "Actors.h"
#include "PowerUp.h"
#include "bomb.h"

#include <vector>
#include <list>

using namespace std;

class bomb;

enum GunFireDir{
	GUN_NONE = 0,
	GUN_N,
	GUN_S,
	GUN_W,
	GUN_E
};
class Character : public Actors{
private:
	//int _movTiles;
	int _speed;
	int _bombs;
	int _fire;
	int _shield;
	bool alive;


	vector<int> _effects;//power-ups that have active effect 
	list<bomb*> entries; //lista de bombas


public:
	int direction;
	bool shot;

	Character();
	~Character();

	void AddPowerUp(PowerUp* entry);
	bool queryPowerUp(int powup);

	void reset();
	void init(int t);

	int getSpeed();
	int getBombs();
	int getFire();
	int getShield();

	void setShield(int s);
	void AddShield(int s);

	bool haveBomb();
	void detonate();

	void AddEntry(bomb* b);
	void RemoveEntry(int x, int y);

	void setAlive(bool b);
	bool getAlive();

};
#endif