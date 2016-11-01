#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Stage.h"
#include "../engine/renderer.h"
#include <vector>
#include <queue>

using namespace std;

class userInterface
{
private:
	Stage* _stg;
	TileMap* _tileMap;
	Team *_teams[2];

	Actors* _selected;
	Actors* _target;
	queue<int> _commands;



	vector<Actors> _overlay;
	Actors _ohi;//Over Head Icon

	Button* _endturn;

	void pushOvelay(int x, int y, int ol);
	
public:
	queue<string> _actionMsg;


	userInterface();
	~userInterface();


	void init(Stage* stg);


	matMN<Tile>& getTileMap();
	bool areEnemies(Character* c1, Character* c2);

	Actors* getActorAt(int x, int y);
	Actors* getOverlayAt(int x, int y);

	void update(int xm, int ym, int player, int val = 0);

	void updateCommand(int xm, int ym, int player);
	void initCommand();
	void fillCommands(int player);
	void clearCommands();
	void placeCommands(int cmd);

	void setOHI(int name, int x, int y);

	void clearOverlays();

	void markEntry(Actors* entry);
	void markWalk(Character* entry);
	void markBomb(Character* entry);
	void markGunFire(Character* entry);
	void markTimerUp(Character* entry);
	void markTimerDown(Character* entry);
	void markThrow(Character* entry);
	void markDetonator(Character* entry);
	void markBarrel(Character* entry);
	void markRope(Character* entry);


	void renderOverlay(renderer& ren, camera& cam);
	void renderIcons(renderer& ren, camera& cam);
	void clear();

};

#endif