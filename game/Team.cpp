#include "Team.h"


charState::charState(){
	for(int i=0;i<ACTIONSIZE;++i)
		_state[i] = STATE_READY;
}
charState::~charState(){
}

void charState::reset(){
	for(int i=0;i<ACTIONSIZE;++i)
		_state[i] = STATE_READY;
}

void charState::update(){
	for(int i=0;i<ACTIONSIZE;++i)
		if(_state[i] > 0)
			--_state[i];
	//TODO solve update state type moves
	/*if(_state[ACTIONGUNFIRE] == STATE_STAGE1)
		_state[ACTIONGUNFIRE] = STATE_FIRE;
	else if(_state[ACTIONGUNFIRE] == STATE_STAGE2)
		_state[ACTIONGUNFIRE] = STATE_READY;*/

}

void charState::addCooldown(int action, int time){
	if(_state[action] >= STATE_READY)
		_state[action] += time;
	else
		_state[action] = time;
}
void charState::setState(int action, int state){
	_state[action] = state;
}
int charState::getState(int action){
	return _state[action];
}

bool charState::canUse(int action){
	if(_state[action] == STATE_READY)
		return true;
	return false;
}



Team::Team()
{
	_coin = 0;
	for (int i = 0; i < 5; i++){
		actions[i] = CHAR_NONE;
	}
}
Team::~Team()
{
}

void Team::beginTurn(){
	for (int i = 0; i < 5; i++){
		_state[i].update();
		if(_state[i].canUse(ACTIONGUNFIRE)){
			actions[i] = CHAR_NONE;
		}
	}
}

//verifica se ta no time, e retorna quem
int Team::checkSelected(Character* c){
	for(int i=0;i<5;++i){
		if(c == &_characters[i]){
			return i;
		}
	}
	return -1; // retorna alguem que nao pode mover
}

void Team::setCoin(int coin){
	_coin = coin;
}
void Team::addCoin(int coin){
	_coin += coin;
}

int Team::getCoin(){
	return _coin;
}

Character& Team::getCharacter(int pos){
	return _characters[pos];
}

void Team::setCharacter(int pos, Character c){
	_characters[pos] = c;
}

int Team::isMember(Character* c){
	for(int i=0;i<5;++i)
		if(&_characters[i] == c)
			return i;
	return -1;
}

void Team::reset(){
	setCoin(1000);
	for(int i = 0; i < 5; i++){
		getCharacter(i).reset();
		_state[i].reset();
		actions[i] = CHAR_NONE;
	}
	count = 5;
}


