#include "TileMap.h"
#include "Stage.h"

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

void TileMap::encode(char* data, int& len){
	int p=-1;
	data[++p] = 0;//2byte size
	data[++p] = 0;//2byte size

	for(int i=0;i<_map.width()*_map.height();++i){
		char type=-1;
		char index=-1;
		if(_map.at(i).actor != NULL){
			type = _map.at(i).actor->getClass();
			index = _map.at(i).actor->getIndex();
		}
		data[++p] = type;
		data[++p] = index;
	}
	short* ss=(short*)data;
	(*ss) = ++p;
	len = p;
}
void TileMap::decode(char* data, Stage* stg){
	int p=1;
	for(int i=0;i<_map.width()*_map.height();++i){
		char type = data[++p];
		char index = data[++p];

		if(type == ACTOR_BOMB){
			_map.at(i).actor = stg->_bombs[index];
		}else  if(type == ACTOR_BLOCK){
			_map.at(i).actor = stg->_blocks[index];
		}else  if(type == ACTOR_POWUP){
			_map.at(i).actor = stg->_powerUps[index];
		}else{
			_map.at(i).actor = NULL;
		}
	}
}





