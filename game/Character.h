#ifndef CHARACTER_H
#define CHARACTER_H

#include "Actors.h"
#include "PowerUp.h"

#include <vector>
#include <list>

using namespace std;

static const int MAX_BOMBS = 6;

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
	int _speed;//movment
	int _capacity;//number of max bombs
	int _power;//dynamite power
	int _shield;
	bool _alive;

	bool _effects[ACTIVE_POWUP_SIZE];//power-ups that have active effect 
	int _bombs[MAX_BOMBS];//index of bombs instanced]
	int _size;//current number of bombs instanced


public:
	int _dir;

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
	void detonate(int* child);

	bool AddDynamite(int index);
	void RemoveEntry(int index);

	void setAlive(bool b);
	bool getAlive();

	Character* getCharacter();

	void encode(char* data, int& len);
	void decode(char* data);
};
#endif