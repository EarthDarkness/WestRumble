#include "bomb.h"
#include "enum.h"

extern animation BMB_S_0;
extern animation BMB_S_30;
extern animation BMB_S_80;
extern animation BMB_S_180;
extern animation BMB_S_250;
extern animation BMB_S_300;

extern animation BMB_T_0;
extern animation BMB_T_30;
extern animation BMB_T_80;
extern animation BMB_T_180;
extern animation BMB_T_250;
extern animation BMB_T_300;

extern animation BMB_H_0;
extern animation BMB_H_30;
extern animation BMB_H_80;
extern animation BMB_H_180;
extern animation BMB_H_250;
extern animation BMB_H_300;

bomb::bomb(){
	_class = ACTOR_BOMB;

	_turns = DEF_BOMB_TIMER;
	_fire = 0;

	_index = -1;
	_team = -1;
	_owner = -1;
}
bomb::~bomb(){
}


void bomb::init(int fire, int index, int team, int owner){
	_fire = fire;

	_index = index;
	_team = team;
	_owner = owner;

	int r = rand()%6;
	cout << "BOMB: " << _fire << "-" << r << endl;
	if(_fire <= 2){
		if(r == 0){
			_sprite.init(BMB_S_0);
		}else if(r == 1){
			_sprite.init(BMB_S_30);
		}else if(r == 2){
			_sprite.init(BMB_S_80);
		}else if(r == 3){
			_sprite.init(BMB_S_180);
		}else if(r == 4){
			_sprite.init(BMB_S_250);
		}else if(r == 5){
			_sprite.init(BMB_S_300);
		}
	}else if(_fire <= 4){
		if(r == 0){
			_sprite.init(BMB_T_0);
		}else if(r == 1){
			_sprite.init(BMB_T_30);
		}else if(r == 2){
			_sprite.init(BMB_T_80);
		}else if(r == 3){
			_sprite.init(BMB_T_180);
		}else if(r == 4){
			_sprite.init(BMB_T_250);
		}else if(r == 5){
			_sprite.init(BMB_T_300);
		}
	}else if(_fire >4){
		if(r == 0){
			_sprite.init(BMB_H_0);
		}else if(r == 1){
			_sprite.init(BMB_H_30);
		}else if(r == 2){
			_sprite.init(BMB_H_80);
		}else if(r == 3){
			_sprite.init(BMB_H_180);
		}else if(r == 4){
			_sprite.init(BMB_H_250);
		}else if(r == 5){
			_sprite.init(BMB_H_300);
		}
	}
	//TODO set initial state
	//_sprite.setState("idle");
}


void bomb::setFire(int fire){
	_fire = fire;
}
int bomb::getFire(){
	return _fire;
}

void bomb::addTurn(int val){
	_turns += val;
}
void bomb::setTurn(int val){
	_turns = val;
}
int bomb::getTurn(){
	return _turns;
}

int bomb::getindex(){
	return _index;
}
int bomb::getTeam(){
	return _team;
}
int bomb::getOwner(){
	return _owner;
}

bool bomb::checkTurnUp(){
	if (_turns < MAX_BOMB_TIMER){
		return true;
	}
	return false;
}
bool bomb::checkTurnDown(){
	if (_turns > 1){
		return true;
	}
	return false;
}