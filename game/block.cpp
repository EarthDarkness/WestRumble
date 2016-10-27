#include "block.h"
#include "Props.h"

block::block(){
	_class = ACTOR_BLOCK;

	_data = 0;
	
}
block::~block(){
}

void block::setData(int data){
	_data = data;
}
int block::getData(){
	return _data;
}

block* block::getBlock(){
	return dynamic_cast<block*>(this);
}

void block::encode(char* data, int& len){
	int p=-1;
	data[++p] = 0;

	data[++p] = _class & 0xff;
	data[++p] = _x & 0xff;
	data[++p] = _y & 0xff;
	data[++p] = _anim & 0xff;

	data[++p] = _data & 0xff;

	data[0] = ++p;
	len = p;
}
void block::decode(char* data){
	int p=0;
	if(data[1] != ACTOR_BLOCK)
		return;

	_class = data[++p];
	_x = data[++p];
	_y = data[++p];
	_anim = data[++p];
	setAnimation(BLK_TABLE,_anim);

	_data = data[++p];
}

