#include "input.h"


input::input(){
	reset();
}
input::~input(){
}

void input::update(bool down, s32 x, s32 y){
	if(down){
		if(_state == 0)
			_state = 2;	//pressed
	}else{
		_state = 0;	//up
	}

	_xPos = x;
	_yPos = y;
}

bool input::isPress(){
	if(_state == 2){
		_state = 1;
		return true;
	}else{
		return false;
	}
}
bool input::isUp(){
	if(_state == 0){
		return true;
	}else{
		return false;
	}
}
bool input::isDown(){
	if(_state == 1 || _state == 2){
		return true;
	}else{
		return false;
	}
}

	
s32 input::getX(){
	return _xPos;
}
s32 input::getY(){
	return _yPos;
}

void input::reset(){
	_state = 0;
	_xPos = 0;
	_yPos = 0;
}