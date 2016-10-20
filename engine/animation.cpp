#include "animation.h"
#include "translator.h"

#include <sstream>
#include <vector>
#include <memory.h>

using namespace std;

animation::animation(){
	_state = 0;
	memset(_index,0,sizeof(_index));
}
animation::~animation(){
}


void animation::init(animation& src){
	for(int i=0;i<MAX_ANIMS;++i){
		_index[i] = src._index[i];
		_frame[i]._loop = src._frame[i]._loop;

		_frame[i].setSpeed(src._frame[i].getSpeed());
	}
}

int animation::get(){
	return _index[_state]+_frame[_state].get();
}

bool animation::isDone(){
	return _frame[_state].isDone();
}

void animation::setState(int stt){
	_frame[_state].stop();
	_state = stt;
	_frame[_state].play();
}

int animation::getState(){
	return _state;
}


void animation::loadAnim(int stt, int index, int size, float speed, bool loop){
	_index[stt] = index;
	_frame[stt].init(size);
	_frame[stt].setSpeed(speed);
	_frame[stt]._loop = loop;
}




