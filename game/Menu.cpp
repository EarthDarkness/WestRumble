#include "Menu.h"
#include "enum.h"
#include "Props.h"

Menu::Menu(){
	_state = MAIN_MENU;

	_done = false;

	_logo.image_id = 0;

	_play.image_id = 0;
	_tutorial.image_id = 0;
	_credits.image_id = 0;

	_next.image_id = 0;
	_prev.image_id = 0;
	_back.image_id = 0;

	_info[0].image_id = 0;
	_info[1].image_id = 0;
	_info[2].image_id = 0;
	_info[3].image_id = 0;

	_page = 0;

	_net = 0;//0 no, 1 server, 2 client

}
Menu::~Menu(){
}

void Menu::setupDisplay(){
	guiEntry* ptrArr[8*8];

	//mainmenu
	int p=-1;
	//server client
	ptrArr[++p] = &_server;
	ptrArr[++p] = &_client;
	//menu 
	ptrArr[++p] = &_play;
	ptrArr[++p] = new panel(2, 1, 0.1f, 0.0f, ptrArr);
	ptrArr[++p] = &_tutorial;
	ptrArr[++p] = &_credits;
	//spaing
	ptrArr[++p] = NULL;
	ptrArr[++p] = new panel(1, 4, 0.0f, 0.1f, ptrArr+2);
	ptrArr[++p] = NULL;

	_display[MAIN_MENU].init(3, 1, 0.0f, 0.0f, ptrArr+6);

}

void Menu::init(){
	_state = MAIN_MENU;

	setupDisplay();

	//_numpad.init();

	_itemTable[MAIN_MENU].push_back(&_logo);
	_itemTable[MAIN_MENU].push_back(&_play);
	_itemTable[MAIN_MENU].push_back(&_server);
	_itemTable[MAIN_MENU].push_back(&_client);
	_itemTable[MAIN_MENU].push_back(&_tutorial);
	_itemTable[MAIN_MENU].push_back(&_credits);

	_itemTable[TUTORIAL_MENU].push_back(&_next);
	_itemTable[TUTORIAL_MENU].push_back(&_prev);
	_itemTable[TUTORIAL_MENU].push_back(&_back);

	//for(int i=0;i<10;++i)
	//	_itemTable[MULTIPLAYER_MENU].push_back(&(_numpad._nums[i]));

	_logo.image_id = IMG_GFX::logo;

	_play.image_id = IMG_UI::play;
	_server.image_id = IMG_UI::server;
	_client.image_id = IMG_UI::client;
	_tutorial.image_id = IMG_UI::tutorial;
	_credits.image_id = IMG_UI::credits;

	_next.image_id = IMG_UI::move;
	_prev.image_id = IMG_UI::move;
	_back.image_id = IMG_UI::exit;

	_info[0].image_id = IMG_TUT::objectives;
	_info[1].image_id = IMG_TUT::controls;
	_info[2].image_id = IMG_TUT::rules;
	_info[3].image_id = IMG_TUT::actions;



	_page = 0;
}

void Menu::setGUI(int width, int height){
	_display[MAIN_MENU].setGUI(0,0,width,height);

	//int lw = 840;
	//int lh = 420;
	//scaleHeight(height/2,lw,lh);
	//if(lw>width)
	//	scaleWidth(width,lw,lh);
	//_logo.setPos(width/2-lw/2,0,lw,lh);

	//int ss;//space size
	//int is;//item size
	//fitInSpace(height/2,4,0.9f,ss,is);

	//lw = 450;
	//lh = 180;
	//scaleHeight(is,lw,lh);
	//if(lw>width)
	//	scaleWidth(width,lw,lh);

	//int xx = width/2-lw/2;

	//_play.setPos(xx,height/2+ss/2+0*(lh+ss),lw,lh);
	//_server.setPos(xx-lw/2,height/2+ss/2+1*(lh+ss),lw,lh);
	//_client.setPos(xx+lw/2,height/2+ss/2+1*(lh+ss),lw,lh);
	//_tutorial.setPos(xx,height/2+ss/2+2*(lh+ss),lw,lh);
	//_credits.setPos(xx,height/2+ss/2+3*(lh+ss),lw,lh);
	//
	//bool portrait = (width<height);
	//
	//_next.setPos(width-100,0,100,100);
	//_prev.setPos(0,0,100,100);
	//_back.setPos(width-96,height-96,96,96);
	//for(int i=0;i<4;++i){
	//	if(portrait){
	//		_info[i].setPos(0,height/2-width/2,width,width);
	//	}else{
	//		_info[i].setPos(width/2-height/2,0,height,height);
	//	}
	//}
	////multiplayer
	////numpad
	//int mh = (float)height*0.3f;
	//int mw = (float)mh*0.75f;
	//int mx = (width-mw)/2;
	//int my = height/2;
	//_numpad.setCoords(mx,my,mw,mh);
}

void Menu::udpdate(int mx, int my){
	if(_state == MAIN_MENU){
		if(_play.checkCollision(mx,my)){
			_done = true;
			//_state = STAGE_MENU;
		}else if(_tutorial.checkCollision(mx,my)){
			_state = TUTORIAL_MENU;
			_page = 0;
		}else if(_server.checkCollision(mx,my)){
			_state = MULTIPLAYER_MENU;
			/*if(_net == 0){
				_net = 1;
			}*/
		}else if(_client.checkCollision(mx,my)){
			_state = MULTIPLAYER_MENU;
			/*if(_net == 0){
				_net = 2;
			}*/
		}
	}else if(_state == STAGE_MENU){
		//_done = true;
	}else if(_state == TUTORIAL_MENU){
		if(_next.checkCollision(mx,my)){
			++_page;
		}else if(_prev.checkCollision(mx,my)){
			--_page;
		}else if(_back.checkCollision(mx,my)){
			_state = MAIN_MENU;
		}
		_page = (_page+4)%4;
	}
}
void Menu::render(renderer& ren){
	for(int i=0;i<_itemTable[_state].size();++i){
		button* ptr = _itemTable[_state][i];
		ren.renderSprite(ptr->image_id,ptr->rect.x,ptr->rect.y,ptr->rect.w,ptr->rect.h);
	}
}

bool Menu::isDone(){
	return _done;
}




