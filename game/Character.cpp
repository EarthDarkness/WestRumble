#include "Character.h"
#include "..\engine\animation.h"

extern animation CHR_char_P1;
extern animation CHR_char_P2;

Character::Character(){
	_class = ACTOR_CHAR;

	_speed = 2;
	_capacity = 1;
	_power = 2;
	_shield = 0;
	alive = true;

	for(int i=0;i<ACTIVE_POWUP_SIZE;++i)
		_effects[i] = false;
	for(int i=0;i<MAX_BOMBS;++i)
		_bombs[i] = -1;

	direction = GUN_NONE;
	shot = false;
}
Character::~Character(){
}

void Character::reset(){
	_class = ACTOR_CHAR;

	_speed = 2;
	_capacity = 1;
	_power = 2;
	_shield = 0;
	alive = true;

	for(int i=0;i<ACTIVE_POWUP_SIZE;++i)
		_effects[i] = false;
	for(int i=0;i<MAX_BOMBS;++i)
		_bombs[i] = -1;

	direction = GUN_NONE;
	shot = false;
}
void Character::init(int t){
	if(t==0)
		_sprite.init(CHR_char_P1);
	else
		_sprite.init(CHR_char_P2);
	//TODO set initial state 
	//_sprite.setState("idle_down");
}

void Character::AddPowerUp(PowerUp* entry){
	int type = entry->getType();
	if(type < POWUP_ACTIVE){
		if(type == POWUP_SPEED)
			_speed += 1;
		else if(type == POWUP_BOMB)
			_capacity += 1;
		else if(type == POWUP_FIRE)
			_power += 1;
		else if(type == POWUP_SHIELD)
			_shield += 1;
	}

	if (type > POWUP_ACTIVE && type < POWUP_END){
		_effects[ACTIVE_POWUP_OFFSET+type] = true;
	}
}
bool Character::queryPowerUp(int powup){
	return _effects[ACTIVE_POWUP_OFFSET+powup];
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
	return _power;
}
int Character::getFire(){
	return _power;
}
int Character::getShield(){
	return _shield;
}

bool Character::AddDynamite(int index){
	if(_size >= MAX_BOMBS)
		return false;
	for(int i=0;i<MAX_BOMBS;++i){
		if(_bombs[i] == -1){
			_bombs[i] = index;
			++_size;
			return true;
		}
	}
	//unexpected
	return false;
}
void Character::RemoveEntry(int index){
	if(_bombs[index] == -1)
		return;

	_bombs[index] = -1;
	--_size;
}

bool Character::haveBomb(){
	if (_capacity > _size)
		return true;
	return false;
}
void Character::detonate(int* child){
	for(int i=0;i<MAX_BOMBS;++i)
		child[i] = _bombs[i];
}

void Character::setAlive(bool b){
	alive = b;
}
bool Character::getAlive(){
	return alive;
}

Character* Character::getCharacter(){
	return reinterpret_cast<Character*>(this);
}
