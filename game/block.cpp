#include "block.h"

block::block(){
	_class = ACTOR_BLOCK;

	_data = 0;
	setSprite("BLK_barrel");
}
block::~block(){
}

void block::setData(int data){
	_data = data;
}
int block::getData(){
	return _data;
}