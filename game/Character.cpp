#include "Character.h"
#include "..\engine\animation.h"
#include "Props.h"

extern animation CHR_char_P1;
extern animation CHR_char_P2;

static int CHR_BASE_SPEED = 4;
static int CHR_BASE_POWER = 3;

Character::Character(){
	_class = ACTOR_CHAR;

	_speed = CHR_BASE_SPEED;
	_capacity = 1;
	_power = CHR_BASE_POWER;
	_shield = 0;
	_alive = true;

	for(int i=0;i<ACTIVE_POWUP_SIZE;++i)
		_effects[i] = false;
	for(int i=0;i<MAX_BOMBS;++i)
		_bombs[i] = -1;

	_dir = GUN_NONE;
}
Character::~Character(){
}

void Character::reset(){
	_class = ACTOR_CHAR;

	_speed = CHR_BASE_SPEED;
	_capacity = 1;
	_power = CHR_BASE_POWER;
	_shield = 0;
	_alive = true;

	for(int i=0;i<ACTIVE_POWUP_SIZE;++i)
		_effects[i] = false;
	for(int i=0;i<MAX_BOMBS;++i)
		_bombs[i] = -1;

	_sprite.setState(ANIM_IDLE_D);

	_dir = GUN_NONE;
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
			_speed += 2;
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
	return _capacity;
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
	for(int i=0;i<MAX_BOMBS;++i){
		if(_bombs[i] == index){
			_bombs[i] = -1;
			--_size;
			return;
		}
	}
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
	_alive = b;
}
bool Character::getAlive(){
	return _alive;
}

Character* Character::getCharacter(){
	return reinterpret_cast<Character*>(this);
}


void Character::encode(char* data, int& len){
	int p=-1;
	data[++p] = 0;

	data[++p] = _class & 0xff;
	data[++p] = _x & 0xff;
	data[++p] = _y & 0xff;
	data[++p] = _sprite.getState() & 0xff;


	data[++p] = _speed & 0xff;
	data[++p] = _capacity & 0xff;
	data[++p] = _power & 0xff;
	data[++p] = _shield & 0xff;
	data[++p] = _alive & 0xff;

	for(int i=0;i<ACTIVE_POWUP_SIZE;++i)
		data[++p] = _effects[i] & 0xff;

	data[++p] = _size & 0xff;
	for(int i=0;i<MAX_BOMBS;++i)
		data[++p] = _bombs[i] & 0xff;

	data[++p] = _dir & 0xff;
	data[0] = ++p;
	len = p;
}
void Character::decode(char* data){
	int p=0;
	if(data[1] != ACTOR_CHAR)
		return;

	_class = data[++p];
	_x = data[++p];
	_y = data[++p];
	_sprite.setState(data[++p]);

	_speed = data[++p];
	_capacity = data[++p];
	_power = data[++p];
	_shield = data[++p];
	_alive = data[++p];

	for(int i=0;i<ACTIVE_POWUP_SIZE;++i)
		_effects[i] = data[++p];

	_size = data[++p];
	for(int i=0;i<MAX_BOMBS;++i)
		_bombs[i] = data[++p];

	_dir = data[++p];
}