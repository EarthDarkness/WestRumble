#include "Actors.h"
#include "Props.h"

Actors::Actors(){
	_class = ACTOR_ACTOR;

	_x = 0;
	_y = 0;

	_anim = 0;

	_index = -1;
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

void Actors::activate(int index){
	if(_index == -1)
		_index = index;
}
void Actors::deactivate(){
	_index = -1;
}
int Actors::getIndex(){
	return _index;
}
bool Actors::isActive(){
	return _index >= 0;
}

block* Actors::getBlock(){
	return NULL;
}
bomb* Actors::getBomb(){
	return NULL;
}
Character* Actors::getCharacter(){
	return NULL;
}
PowerUp* Actors::getPowerUp(){
	return NULL;
}

