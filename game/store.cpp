#include "store.h"
#include "../engine/translator.h"

#include <iostream>

/*extern animation CHR_char_P1;
extern animation CHR_char_P2;

extern animation PWP_barrel;
extern animation PWP_boot;
extern animation PWP_detonator;
extern animation PWP_dynamite;
extern animation PWP_fuse;
extern animation PWP_glove;
extern animation PWP_gunpowder;
extern animation PWP_rope;
extern animation PWP_star;*/

using namespace std;

goodsData::goodsData(){
	_price = 0;
	_stock = 0;
}
goodsData::~goodsData(){
}
void goodsData::set(int x, int y, int w, int h, int price, int powup, int stock = -1){
	_price = price;
	_stock = stock;
	_gui.setPos(x,y,w,h);
	_data.init(powup);
}

store::store(){
	_client = NULL;
	for(int i=0;i<5;++i)
		_player[i].image_id = "NULL";
	
	_selected = 0;
	_done = false;

	_x = 0;
	_y = 0;
}
store::~store(){
}

void store::setGUI(int width, int height){
	int dpi = 20;
	int ss = (height<width?height:width)/dpi;
	int space = ss/4;
	int dist = ss*3;
	int sw = width-2*(space+ss+dist);
	int sh = height-2*(space+ss+dist);

	float ac = 0.75f;
	float pc = 0.9f;

	int md = width<height?width:height;
	int aa = (int)((float)md*ac)/5;
	int spa = (int)((float)md*(1.0f-ac))/6;

	int hh = width<height?(int)(aa/0.75f):aa;
	int ww = width<height?aa:(int)(aa*0.75f);

	int uwt = width<height?width:width-spa*2-ww-spa*2-aa;
	int uht = width<height?height-spa*2-hh-spa*2-aa:height;

	int uv = (uht<uwt)?uht:uwt;
	int vv = (int)((float)uv*pc)/3;
	int spv = (int)((float)uv*(1.0f-pc))/4;
	int xv = width<height?0:spa*2+ww;
	int yv = width<height?spa*2+hh:0;

	_exit.image_id = "exit.png";
	_exit.setPos(width-aa-spa,height-aa-spa,aa,aa);

	for(int i=0;i<5;++i){
		//_player[i].image_id = "NULL";
		if(width<height)
			_player[i].setPos(spa+i*(spa+ww),spa,ww,hh);
		else
			_player[i].setPos(spa,spa+i*(spa+hh),ww,hh);
	}
	for(int j=0;j<3;++j){
		for(int i=0;i<3;++i){
			_goods[j*3+i]._gui.setPos(xv+spv+i*(spv+vv),yv+spv+j*(spv+vv)+vv/4,vv,vv/2);
		}
	}

	//_x = width-aa-spa+(aa-96)/2-8;
	//_y = height-aa-spa-(aa-32)/2;
	_x = width-aa-spa*2-96;
	_y = height-aa/2-spa-12;

}
void store::load(resources& res){


	//button_end_turn.setPos(width_screen-dist-ss/2,height_screen-dist-ss/2,dist,dist);

	//res.loadImage("resources/image/bomb.png","bomb.png");
	//res.loadImage("resources/image/boot.png","boot.png");
	//res.loadImage("resources/image/fire.png","fire.png");

	res.loadImage("resources/image/UI_char_black.png","UI_char_black");
	res.loadImage("resources/image/UI_char_white.png","UI_char_white");
	res.loadImage("resources/image/select.png","select.png");

	res.loadImage("resources/image/exit.png","exit.png");


	//_exit.image_id = "exit.png";
	//_exit.setPos(700,500,48,48);
	//_exit.setPos(0,0,48,48);
	/*
	for(int i=0;i<5;++i){
		_player[i].image_id = "char.png";
		_player[i].setPos(50,50+i*100,48,64);
	}*/

	//get<0>(_goods[0]).image_id = "bomb.png";
	//get<0>(_goods[1]).image_id = "boot.png";
	//get<0>(_goods[2]).image_id = "fire.png";
	/*_goods[0]._gui.image_id = "bomb.png";
	_goods[1]._gui.image_id = "boot.png";
	_goods[2]._gui.image_id = "fire.png";*/
	_goods[0]._gui.image_id = "NULL";
	_goods[1]._gui.image_id = "NULL";
	_goods[2]._gui.image_id = "NULL";
	_goods[3]._gui.image_id = "NULL";
	_goods[4]._gui.image_id = "NULL";
	_goods[5]._gui.image_id = "NULL";
	_goods[6]._gui.image_id = "NULL";
	_goods[7]._gui.image_id = "NULL";
	_goods[8]._gui.image_id = "NULL";
	/*for(int i=0;i<3;++i){
		_goods[i]._gui.setPos(400,200+i*96,48,48);
		//get<0>(_goods[i]).setPos(400,200+i*96,48,48);
	}*/
	_goods[0]._price = 100;
	_goods[1]._price = 50;
	_goods[2]._price = 150;
	_goods[0]._data.init(POWUP_BOMB);
	_goods[1]._data.init(POWUP_SPEED);
	_goods[2]._data.init(POWUP_FIRE);
	_goods[3]._price = 200;
	_goods[4]._price = 150;
	_goods[5]._price = 150;
	_goods[3]._data.init(POWUP_SHIELD);
	_goods[4]._data.init(POWUP_BOMB_RELAUNCH);
	_goods[5]._data.init(POWUP_BOMB_TIMER);
	_goods[6]._price = 250;
	_goods[7]._price = 200;
	_goods[8]._price = 250;
	_goods[6]._data.init(POWUP_DETONATOR);
	_goods[7]._data.init(POWUP_BARRIER);
	_goods[8]._data.init(POWUP_STUN);
	//get<1>(_goods[0]) = 100;
	//get<1>(_goods[1]) = 50;
	//get<1>(_goods[2]) = 150;
	//get<2>(_goods[0]).init(POWUP_BOMB);
	//get<2>(_goods[1]).init(POWUP_SPEED);
	//get<2>(_goods[2]).init(POWUP_FIRE);
	_goods[0]._stock = 6;
	_goods[1]._stock = 6;
	_goods[2]._stock = 6;
	_goods[3]._stock = 2;
	_goods[4]._stock = 4;
	_goods[5]._stock = 3;
	_goods[6]._stock = 2;
	_goods[7]._stock = 3;
	_goods[8]._stock = 3;


}

void store::init(Team* client, int color){
	_done = false;
	_client = client;

	_selected = 0;
	
	for(int i=0;i<5;++i){
		if(color == 0){
			_player[i].image_id = "UI_char_black";
		}else{
			_player[i].image_id = "UI_char_white";
		}
	}

	_goods[0]._stock = 6;
	_goods[1]._stock = 6;
	_goods[2]._stock = 6;
	_goods[3]._stock = 2;
	_goods[4]._stock = 4;
	_goods[5]._stock = 3;
	_goods[6]._stock = 2;
	_goods[7]._stock = 3;
	_goods[8]._stock = 3;
}
void store::update(core& engine){
	if(_done)
		return;
	if(_client == NULL){
		cout << "no _client set to buy on the store" << endl;
		return;
	}

	if(!engine._input.isPress())
		return;

	if(_exit.checkCollision(engine._input.getX(),engine._input.getY())){
		_done = true;
		engine._render.renderSprite("L1",350,350);
		return;
	}
	for(int i=0;i<5;++i){
		if(_player[i].checkCollision(engine._input.getX(),engine._input.getY())){
			_selected = i;
			return;
		}
	}
	/*for(map<Button,pair<int,PowerUp>>::iterator it = _goods.begin();it != _goods.end();++it){
		if(it->first.checkCollision(engine._input.getX(),engine._input.getY())){
			if(_client->getCoin() < it->second.first)
				return;
			_client->addCoin(-it->second.first);
			_client->getCharacter(_selected).AddPowerUp(&(it->second.second));
			return;
		}
	}*/

	if(_selected == -1)
		return;

	/*for(int i=0;i<3;++i){
		if(get<0>(_goods[i]).checkCollision(engine._input.getX(),engine._input.getY())){
			if(_client->getCoin() < get<1>(_goods[i]))
				return;
			_client->addCoin(-get<1>(_goods[i]));
			_client->getCharacter(_selected).AddPowerUp(&get<2>(_goods[i]));
			return;
		}
	}*/

	for(int i=0;i<9;++i){
		if(_goods[i]._gui.checkCollision(engine._input.getX(),engine._input.getY())){
			if(_goods[i]._stock <= 0 && _goods[i]._stock != -1)
				return;
			if(_client->getCoin() < _goods[i]._price)
				return;
			_client->addCoin(-_goods[i]._price);
			_client->getCharacter(_selected).AddPowerUp(&_goods[i]._data);
			if(_goods[i]._stock > 0)
				_goods[i]._stock -= 1;
			return;
		}
	}
}
void store::render(core& engine){
	engine._render.renderSprite(_exit.image_id.c_str(),_exit.rect.x,_exit.rect.y,_exit.rect.w,_exit.rect.h);
	for(int i=0;i<5;++i)
		engine._render.renderSprite(_player[i].image_id.c_str(),_player[i].rect.x,_player[i].rect.y,_player[i].rect.w,_player[i].rect.h);
	if(_selected >=0 && _selected <= 5)
		engine._render.renderSprite("select.png",_player[_selected].rect.x,_player[_selected].rect.y,_player[_selected].rect.w,_player[_selected].rect.h);
	for(int i=0;i<9;++i){
		if(_goods[i]._stock == 0)
			continue;
		engine._render.renderSprite(_goods[i]._data.getSprite(),_goods[i]._gui.rect.x,_goods[i]._gui.rect.y,_goods[i]._gui.rect.w,_goods[i]._gui.rect.h);
		engine._render.renderText(i_to_str(_goods[i]._price).c_str(),engine._font,_goods[i]._gui.rect.x+_goods[i]._gui.rect.w/2,_goods[i]._gui.rect.y+_goods[i]._gui.rect.h/2);
		if(_goods[i]._stock > -1)
			engine._render.renderText(i_to_str(_goods[i]._stock).c_str(),engine._font,_goods[i]._gui.rect.x+_goods[i]._gui.rect.w/3,_goods[i]._gui.rect.y);
	}
		//engine._render.renderSprite(_goods[i]._gui.image_id.c_str(),_goods[i]._gui.rect.x,_goods[i]._gui.rect.y,_goods[i]._gui.rect.w,_goods[i]._gui.rect.h);
	engine._render.renderText(i_to_str(_client->getCoin()).c_str(),engine._font,_x,_y);
}

bool store::isDone(){
	return _done;
}