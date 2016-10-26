#include "Actors.h"
#include "Props.h"

Actors::Actors(){
	_class = ACTOR_ACTOR;

	_x = 0;
	_y = 0;

	_anim = 0;
}
Actors::~Actors(){

}

int Actors::getSprite(){
	//return _sprite.c_str();
	return G_Anim[_sprite.get()];
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
//void Actors::setSprite(const char* name){
//	//_sprite.set("default",name);
//	//IMPLEMENT
//}

int Actors::getClass(){
	return _class;
}

void Actors::setAnimation(animation* table[],int id,int stt){
	_anim = id;
	if(id > 0)
		_sprite.init(*table[id]);
	_sprite.setState(stt);
}
animation& Actors::getAnimation(){
	return _sprite;
}

void Actors::setState(int stt){
	_sprite.setState(stt);
}