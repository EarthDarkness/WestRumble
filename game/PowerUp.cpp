#include "PowerUp.h"

extern animation PWP_barrel;
extern animation PWP_boot;
extern animation PWP_detonator;
extern animation PWP_dynamite;
extern animation PWP_fuse;
extern animation PWP_glove;
extern animation PWP_gunpowder;
extern animation PWP_rope;
extern animation PWP_star;

PowerUp::PowerUp(){
	_class = ACTOR_POWUP;

}
PowerUp::~PowerUp(){

}

int PowerUp::getType(){
	return _type;
}

void PowerUp::init(int type){
	if (type == POWUP_SPEED){
		_sprite.init(PWP_boot);
	}else if (type == POWUP_BOMB){
		_sprite.init(PWP_dynamite);
	}else if (type == POWUP_FIRE){
		_sprite.init(PWP_gunpowder);
	}else if (type == POWUP_SHIELD){
		_sprite.init(PWP_star);
	}else if (type == POWUP_BOMB_TIMER){
		_sprite.init(PWP_fuse);
	}else if (type == POWUP_BOMB_RELAUNCH){
		_sprite.init(PWP_glove);
	}else if (type == POWUP_BARRIER){
		_sprite.init(PWP_barrel);
	}else if (type == POWUP_STUN){
		_sprite.init(PWP_rope);
	}else if (type == POWUP_DETONATOR){
		_sprite.init(PWP_detonator);
	}

	if (type > POWUP_NONE && type < POWUP_END && type != POWUP_ACTIVE)
		_type = type;
}


PowerUp* PowerUp::getPowerUp(){
	return reinterpret_cast<PowerUp*>(this);
}