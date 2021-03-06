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
#include <vector>
#include <sstream>
#include "../engine/translator.h"
#include "Button.h"
#include "PowerUp.h"
#include "block.h"
#include "bomb.h"

using namespace std;

template<class T>
int insert(vector<T*>& dst){
	for(int i=0;i<dst.size();++i){
		if(!dst[i]->isActive()){
			dst[i]->activate(i);
			return i;
		}
	}
	dst.push_back(new T());
	dst.back()->activate(dst.size()-1);
	return dst.size()-1;
}
template<class T>
void remove(vector<T*>& src, int pos){
	src[pos]->deactivate();
}

template<class T>
void removeAll(vector<T*>& src){
	for(int i=0;i<src.size();++i){
		src[i]->deactivate();
	}
}

class Stage
{
private:
	int _initPos[2][5][2]; // [qual equipe][qual personagem][qual posicao]

	Team *_teams[2];
	camera _camera;
	font* _font;

	int _suddenDeath;

	vector<Actors> _vfx;

	Actors _star;

public:
	int _turn;
	TileMap _tileMap;	

	vector<bomb*> _bombs;
	vector<block*> _blocks;
	vector<PowerUp*> _powerUps;	

	Stage();
	~Stage();

	void init(font* f);

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

	void populate();
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

	
	int findActorsPtr(Actors* entry, vector<Actors*>& data);
	int findActorsPos(Actors& entry, vector<Actors>& data);

	void checkExplosion(bomb* b, vector<Actors*>& out);
	void checkGunfire(Character* c, vector<Actors*>& out);

	bool waitForVFX();

	void encode(char* data, int& len);
	void decode(char* data);

};
#endif
