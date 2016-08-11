#ifndef TILEMAP_H
#define TILEMAP_H

#include "..\engine\matrixMN.h"
#include <string>
#include "..\engine\resources.h"
#include "Actors.h"

#include <iostream>

#include <list>

using namespace std;

struct Tile{
	Tile(){
		bot = "";
		up = "";
		collision = 0;
		actor = NULL;
	}
	~Tile()
	{

	}

	void setActor(Actors* a){
		actor = a;
	}

	string bot;
	string up;
	int collision;
	Actors* actor;

};

class TileMap
{
public:
	TileMap();
	~TileMap();

	void setTile(Tile t, int x, int y);
	void init(int w, int h);

	bool checkPos(int x, int y);

	//begin deprecated
	bool existPos(int x, int y);

	bool checkPosMove(int x, int y);
	bool checkPosBomb(int x, int y);
	bool checkPosSpecial(int x, int y, int powup_type);
	//end deprecated

	int getGround(int x, int y);
	Actors* getActor(int x, int y);
	bool inBound(int x, int y);

	matMN<Tile> _map;
private:
};

#endif