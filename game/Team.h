#ifndef TEAM_H
#define TEAM_H

#include "Character.h"
#include "enum.h"

enum CharacterAction{
	CHAR_NONE = 0,
	CHAR_MOVED,
	CHAR_COOLDOWN,
	CHAR_END
};
enum stateValues{
	STATE_FIRE = -3,
	STATE_STAGE2,
	STATE_STAGE1,
	STATE_READY = 0
};
class charState
{
private:
	int _state[ACTIONSIZE];

public:
	charState();
	~charState();

	void reset();

	void update();//at bomb turn

	void addCooldown(int action, int time);
	void setState(int action, int state);
	int getState(int action);
	bool canUse(int action);
};


class Team
{
private:
	
	Character _characters[5];
	
	int _coin;

public:
	Team();
	~Team();

	charState _state[5];

	int actions[5];
	int count;

	void beginTurn();

	int checkSelected(Character* c);

	void setCoin(int coin);
	void addCoin(int coin);

	int getCoin();

	Character& getCharacter(int pos);
	void setCharacter(int pos, Character c);

	int isMember(Character* c);

	void reset();

};
#endif