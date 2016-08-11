#ifndef POWERUP_H
#define POWERUP_H

#include "Actors.h"

enum POWERUPTYPE{
	POWUP_NONE = 0,
	POWUP_BOMB,
	POWUP_FIRE,
	POWUP_SPEED,
	POWUP_SHIELD,
	POWUP_ACTIVE,//apenas marcador
	POWUP_BOMB_TIMER,
	POWUP_BOMB_RELAUNCH,
	POWUP_BARRIER,
	POWUP_STUN,
	POWUP_DETONATOR,
	POWUP_END
};

class PowerUp : public Actors
{
private:
	int _type;
public:
	PowerUp();
	~PowerUp();

	void init(int type);

	int getType();
};

#endif