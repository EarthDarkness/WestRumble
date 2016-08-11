#include "TileMap.h"


TileMap::TileMap()
{
}


TileMap::~TileMap()
{
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

//begin deprecated
bool TileMap::checkPos(int x, int y){
	if (x < _map.width() && x >= 0){
		if (y < _map.height() && y >= 0){
			//dentro do mapa
			if (_map.at(x, y).actor == NULL){
				//nao tem actor
				return true;
			}
		}
	}
	return false;
}
bool TileMap::checkPosMove(int x, int y){
	if (x < _map.width() && x >= 0){
		if (y < _map.height() && y >= 0){
			//dentro do mapa
			if (_map.at(x, y).actor == NULL){
				//nao tem actor
				return true;
			}
			else {
				if (_map.at(x, y).actor->getClass() == ACTOR_BOMB){
					//nao move em bomba
					cout << "!!!Tem Bomba" << endl;
					return false;
				}
				else if (_map.at(x, y).actor->getClass() == ACTOR_CHAR){
					//nao move em char]
					cout << "!!!Tem Char" << endl;
					return false;
				}
				else if (_map.at(x, y).actor->getClass() == ACTOR_BLOCK){
					//nao move em block
					cout << "!!!Tem Block" << endl;
					return false;
				}
				else if (_map.at(x, y).actor->getClass() == ACTOR_POWUP){
					//move em power up
					cout << "!!!Tem POWER UP" << endl;
					return true;
				}
			}

		}
	}
	return false;
}

bool TileMap::existPos(int x, int y){
	if(x < _map.width() && x >= 0){
		if(y < _map.height() && y >= 0){
			//dentro do mapa			
			return true;			
		}
	}
	return false;
}
//end deprecated

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



