#include "TileMap.h"


TileMap::TileMap(){
}

TileMap::~TileMap(){
}

void TileMap::setTile(Tile t, int x, int y){
	_map.set(t, x, y);
}

//inicializa com tile padrão
void TileMap::init(int w, int h){
	Tile t;

	_map.init(w, h);
	_map.fill(t);
}

int TileMap::getGround(int x, int y){
	if(inBound(x,y))
		return _map.at(x,y).collision;
	return 0;
}
Actors* TileMap::getActor(int x, int y){
	if(inBound(x,y))
		return _map.at(x,y).actor;
	return NULL;
}
bool TileMap::inBound(int x, int y){
	if(x >= 0 && x < _map.width() && y >= 0 && y < _map.height())
		return true;
	return false;
}



