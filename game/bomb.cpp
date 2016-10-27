#include "bomb.h"
#include "enum.h"
#include "Props.h"

bomb::bomb(){
	_class = ACTOR_BOMB;

	_turns = DEF_BOMB_TIMER;
	_fire = 0;

	_team = -1;
	_owner = -1;
}
bomb::~bomb(){
}

void bomb::reset(){
	_turns = DEF_BOMB_TIMER;
	_fire = 0;

	_team = -1;
	_owner = -1;
}

void bomb::init(int fire, int team, int owner){
	reset();
	_fire = fire;

	_team = team;
	_owner = owner;

	int r = rand()%6;
	cout << "BOMB: " << _fire << "-" << r << endl;
	if(_fire <= 2){
		setAnimation(BMB_TABLE,BMB_ID_SINGLE,r);
	}else if(_fire <= 4){
		setAnimation(BMB_TABLE,BMB_ID_TRIPLE,r);
	}else if(_fire >4){
		setAnimation(BMB_TABLE,BMB_ID_SEPTUPLE,r);
	}
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

bomb* bomb::getBomb(){
	return reinterpret_cast<bomb*>(this);
}

void bomb::encode(char* data, int& len){
	int p=-1;
	data[++p] = 0;

	data[++p] = _class & 0xff;
	data[++p] = _x & 0xff;
	data[++p] = _y & 0xff;
	data[++p] = _anim & 0xff;
	data[++p] = _sprite.getState() & 0xff;

	data[++p] = _turns & 0xff;
	data[++p] = _fire & 0xff;
	data[++p] = _team & 0xff;
	data[++p] = _owner & 0xff;

	data[0] = ++p;
	len = p;
}
void bomb::decode(char* data){
	int p=0;
	if(data[1] != ACTOR_BOMB)
		return;

	_class = data[++p];
	_x = data[++p];
	_y = data[++p];
	_anim = data[++p];
	setAnimation(BMB_TABLE,_anim);
	_sprite.setState(data[++p]);

	_turns = data[++p];
	_fire = data[++p];
	_team = data[++p];
	_owner = data[++p];


}


