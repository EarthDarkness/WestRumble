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
#include "bomb.h"

using namespace std;

//int rand();
/*class StageData
{
public:
	int _turn;
	int _suddenDeath;

	matMN<int> _type;
	matMN<int> _data;



	StageData();
	StageData();
};*/

class Stage
{
private:
	int _initPos[2][5][2]; // [qual equipe][qual personagem][qual posicao]

	Team *_teams[2];
	camera _camera;
	font* _font;

	int _turn;
	int _suddenDeath;

	vector<Actors> _vfx;

public:
	TileMap _tileMap;	
	Stage();
	~Stage();

	void init(font* f);
	//void setGUI(int width, int height);

	int loadStage(const char* path, resources& resources);
	void start(Team *teamA, Team *teamB);
	void renderMap(renderer& renderer);
	void renderActors(renderer& renderer);
	
	void clear();

	int suddenDeath();
	void addTurn();
	void dropBomb(int num);

	matMN<Tile>& getTileMap();
	camera& getCamera();

	void polulate();
	void spawn(int x, int y);

	Actors* getActorAt(int x, int y);
	Actors* getOverlayAt(int x, int y);

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


};
#endif
