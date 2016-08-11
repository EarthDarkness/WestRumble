#include "Character.h"
#include "..\engine\animation.h"

extern animation CHR_char_P1;
extern animation CHR_char_P2;

Character::Character(){
	_class = ACTOR_CHAR;

	_speed = 2;
	_bombs = 1;
	_fire = 2;
	_shield = 0;
	alive = true;

	direction = GUN_NONE;
	shot = false;
}
Character::~Character(){
}

void Character::reset(){
	_class = ACTOR_CHAR;

	_speed = 2;
	_bombs = 1;
	_fire = 2;
	_shield = 0;
	alive = true;

	direction = GUN_NONE;
	shot = false;
	//_sprite = "character.png";
	//_sprite.set("default","character.png");
}
void Character::init(int t){
	if(t==0)
		_sprite.init(CHR_char_P1);
	else
		_sprite.init(CHR_char_P2);

	_sprite.setState("idle_down");
}

void Character::AddPowerUp(PowerUp* entry){
	int type = entry->getType();
	if(type < POWUP_ACTIVE){
		if(type == POWUP_SPEED)
			_speed += 1;
		else if(type == POWUP_BOMB)
			_bombs += 1;
		else if(type == POWUP_FIRE)
			_fire += 1;
		else if(type == POWUP_SHIELD)
			_shield += 1;
	}
	cout << type << endl;
	if (type > POWUP_ACTIVE && type < POWUP_END){
		cout << "type:" << type << endl;
		if(!queryPowerUp(type))
			_effects.push_back(type);
	}
}
bool Character::queryPowerUp(int powup){
	for(int i=0;i<_effects.size();++i)
		if(_effects[i] == powup)
			return true;
	return false;
}

void Character::setShield(int s){
	_shield = s;
}

void Character::AddShield(int s){
	_shield += s;
}

int Character::getSpeed(){
	return _speed;
}

int Character::getBombs(){
	return _bombs;
}

int Character::getFire(){
	return _fire;
}

int Character::getShield(){
	return _shield;
}

void Character::AddEntry(bomb* b){
	entries.push_back(b);
}
void Character::RemoveEntry(int x, int y){
	for (list<bomb*>::iterator it = entries.begin(); it != entries.end(); it++){
		if ((*it)->getX() == x && (*it)->getY() == y){
			entries.erase(it);
			return;
		}
	}

}

bool Character::haveBomb(){
	if (_bombs > entries.size() ){
		return true;
	} 
	return false;
}
void Character::detonate(){
	for(list<bomb*>::iterator it=entries.begin();it!=entries.end();++it){
		(*it)->setTurn(1);
	}
}

void Character::setAlive(bool b){
	alive = b;
}

bool Character::getAlive(){
	return alive;
}