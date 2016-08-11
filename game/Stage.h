//# A
#ifndef STAGE_H
#define STAGE_H

#define _CRT_SECURE_NO_WARNINGS

#include "TileMap.h"
#include "../engine/core.h"
#include "../engine/renderer.h"
#include "../engine/camera.h"
#include "Team.h"
#include <map>
#include <list>
#include <stack>
#include <sstream>
#include "../engine/translator.h"
#include "Button.h"
#include "PowerUp.h"
#include "block.h"

using namespace std;

//int rand();

class Stage
{
private:
	TileMap _tileMap;	
	int _initPos[2][5][2]; // [qual equipe][qual personagem][qual posicao]

	Team *_teams[2];
	camera _camera;
	font* _font;

	int _turn;
	int _suddenDeath;

	Actors _action;

public:
	Stage();
	~Stage();

	void init(font* f);
	//void setGUI(int width, int height);

	int loadStage(const char* path, resources& resources);
	void start(Team *teamA, Team *teamB);
	void render(renderer& renderer);
	
	void clear();

	int suddenDeath();
	void addTurn();
	void dropBomb(int num);
	
	void setAction(const char* name, int x, int y);

	matMN<Tile>& getTileMap();
	camera& getCamera();

	void polulate();
	void spawn(int x, int y);

	Actors* getActorAt(int x, int y);
	Actors* getOverlayAt(int x, int y);
	
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

	void moveActor(int x0, int y0, int x, int y);
	void instantiateActor(Actors* a, int x, int y);

	Team& getTeam(int player);
	bool areEnemies(Character* c1, Character* c2);

	void hitChar(Character* c);
	void hitPowUp(PowerUp* p);
	void hitBlock(block* b);

	//bombas desenhadas
	list<bomb> bombs;
	
	//blocks desenhados
	list<block> blocks;

	//power ups
	list<PowerUp> power_ups;
	
	
	int findActorsPtr(Actors* entry, vector<Actors*>& data);
	int findActorsPos(Actors& entry, vector<Actors>& data);

	void checkExplosion(bomb* b, vector<Actors*>& out);
	void checkGunfire(Character* c, vector<Actors*>& out);

	bool waitForVFX();

private:
	vector<Actors> _overlay;
	vector<Actors> _vfx;


};
#endif
