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
		bot = 0;
		up = 0;
		collision = 0;
		actor = NULL;
	}
	~Tile()
	{

	}

	void setActor(Actors* a){
		actor = a;
	}

	int bot;
	int up;
	int collision;
	Actors* actor;//to index

};

class TileMap
{
public:
	TileMap();
	~TileMap();

	void setTile(Tile t, int x, int y);
	void init(int w, int h);

	int getGround(int x, int y);
	Actors* getActor(int x, int y);
	bool inBound(int x, int y);

	matMN<Tile> _map;
private:
};

#endif