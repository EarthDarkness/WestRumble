#ifndef ENUM_H
#define ENUM_H

//#include <random>
extern int rseed;
#ifndef srand
#define srand(x) rseed=x 
#endif
int randn();

#ifndef rand
#define rand() randn()
#endif

enum GameState{
	STATEMENU = 0,
	STATESHOP_A,
	STATESHOP_B,
	STATEGAME,
	STATEEND,
	STATEPAUSE
};

enum CharacterActionState{
	ACTIONNULL = 0,			//no action always first
	ACTIONMOVE,				//state: can move or not
	ACTIONBOMB,				//state: can place bomb or not
	ACTIONGUNFIRE,
	ACTIONTIMER_UP,			//state: cooldown to action
	ACTIONTIMER_DOWN,		//state: cooldown to action
	ACTIONRELAUNCHSELECT,
	ACTIONRELAUNCH,
	ACTIONBARRIER,
	ACTIONSTUN,
	ACTIONBOMBDETONATOR,
	ACTIONSIZE				//max actions always last
};


void fitInSpace(int total, int num, float cover, int& space, int& size);
void scaleWidth(int size, int& width, int& height);
void scaleHeight(int size, int& width, int& height);



#endif