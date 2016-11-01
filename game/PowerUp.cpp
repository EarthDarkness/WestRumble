#include "PowerUp.h"
#include "Props.h"


PowerUp::PowerUp(){
	_class = ACTOR_POWUP;

}
PowerUp::~PowerUp(){

}

int PowerUp::getType(){
	return _type;
}

void PowerUp::init(int type){
	/*if (type == POWUP_SPEED){
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
	}*/

	if (type > POWUP_NONE && type < POWUP_END && type != POWUP_ACTIVE)
		_type = type;
}


PowerUp* PowerUp::getPowerUp(){
	return reinterpret_cast<PowerUp*>(this);
}

void PowerUp::encode(char* data, int& len){
	int p=-1;
	data[++p] = 0;

	data[++p] = _class & 0xff;
	data[++p] = _x & 0xff;
	data[++p] = _y & 0xff;
	data[++p] = _anim & 0xff;

	data[++p] = _type & 0xff;

	data[0] = ++p;
	len = p;
}
void PowerUp::decode(char* data){
	int p=0;
	if(data[1] != ACTOR_POWUP)
		return;

	_class = data[++p];
	_x = data[++p];
	_y = data[++p];
	_anim = data[++p];
	setAnimation(PWP_TABLE,_anim);

	_type = data[++p];

}

