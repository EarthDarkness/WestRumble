#include "Actors.h"

Actors::Actors(){
	_class = ACTOR_ACTOR;

	_x = 0;
	_y = 0;

	//_sprite = "NULL";
}
Actors::~Actors(){

}

const char* Actors::getSprite(){
	//return _sprite.c_str();
	return _sprite.get();
}
int Actors::getX(){
	return _x;
}
int Actors::getY(){
	return _y;
}
void Actors::setPos(int x, int y){
	_x = x;
	_y = y;
}
void Actors::setSprite(const char* name){
	_sprite.set("default",name);
}

int Actors::getClass(){
	return _class;
}

animation& Actors::getAnimation(){
	return _sprite;
}
void Actors::setState(const char* name){
	_sprite.setState(name);
}