#include "Menu.h"
#include "enum.h"
#include "Props.h"

void Menu::ipPushNum(int num){
	uint8_t* va = (uint8_t*)(&_mpIpAddr);
	int p = _mpIpPos;

	if((va[p] >= 100) || (va[p] >= 25 && va[p] < 100))
		va[p] = 0;

	va[p]*=10;
	va[p]+=num;

	if((va[p] >= 100) || (va[p] >= 25 && va[p] < 100))
		_mpIpPos = (_mpIpPos+1)%4;
}

Menu::Menu(){
	_state = MAIN_MENU;

	_done = false;

	_page = 0;

	_net = 0;//0 no, 1 server, 2 client
	_mpIpPos = 0;
	_mpIpAddr = 0;

}
Menu::~Menu(){
}

void Menu::setupDisplay(){
	guiEntry* ptrArr[8*8];

	//mainmenu
	int p;

	//title
	p=-1;
	ptrArr[++p] = &_logo;		_logo.init(0.5f,0.25f,0.75f,0.4f,true);
	ptrArr[++p] = &_play;		_play.init(0.5f,0.5f,0.3f,0.1f,true);

	ptrArr[++p] = &_multiplayer;	_multiplayer.init(0.5f,0.6f,0.3f,0.1f,true);
	//ptrArr[++p] = &_server;		_server.init(0.35f,0.6f,0.3f,0.1f,true);
	//ptrArr[++p] = &_client;		_client.init(0.65f,0.6f,0.3f,0.1f,true);

	ptrArr[++p] = &_tutorial;	_tutorial.init(0.5f,0.7f,0.3f,0.1f,true);
	ptrArr[++p] = &_credits;	_credits.init(0.5f,0.8f,0.3f,0.1f,true);

	_display[MAIN_MENU].init(p+1,ptrArr);

	//stage select
	p=-1;
	ptrArr[++p] = &_stgThumb[0];
	ptrArr[++p] = &_stgThumb[1];
	ptrArr[++p] = &_stgThumb[2];
	ptrArr[++p] = &_stgThumb[3];

	ptrArr[++p] = new panel(2,2,0.25f,0.25f,ptrArr);	ptrArr[p]->init(0.5f,0.4f,0.7f,0.7f,true);
	ptrArr[++p] = &_stgNext;							_stgNext.init(0.35f,0.8f,0.3f,0.1f,true);
	ptrArr[++p] = &_stgPrev;							_stgPrev.init(0.65f,0.8f,0.3f,0.1f,true);

	_display[STAGE_MENU].init(3,ptrArr+4);

	//multiplayer menu
	p=-1;
	for(int i=1;i<10;++i)
		ptrArr[++p] = &_mpNumpad[i];
	ptrArr[++p] = NULL;
	ptrArr[++p] = &_mpNumpad[0];
	ptrArr[++p] = NULL;

	ptrArr[++p] = new panel(3,4,0.4f,0.4f,ptrArr);	ptrArr[p]->init(0.3f,0.6f,0.4f,0.45f,true);

	ptrArr[++p] = &_mpServer;						_mpServer.init(0.35f,0.1f,0.3f,0.1f,true);
	ptrArr[++p] = &_mpClient;						_mpClient.init(0.65f,0.1f,0.3f,0.1f,true);

	ptrArr[++p] = &_mpLeft;							_mpLeft.init(0.15f,0.25f,0.1f,0.1f,true);
	ptrArr[++p] = &_mpIp;							_mpIp.init(0.5f,0.25f,0.5f,0.1f,true);
	ptrArr[++p] = &_mpRight;						_mpRight.init(0.85f,0.25f,0.1f,0.1f,true);

	ptrArr[++p] = &_mpPool;							_mpPool.init(0.7f,0.6f,0.4f,0.45f,true);

	ptrArr[++p] = &_mpNext;							_mpNext.init(0.35f,0.9f,0.3f,0.1f,true);
	ptrArr[++p] = &_mpPrev;							_mpPrev.init(0.65f,0.9f,0.1f,0.1f,true);

	_display[MULTIPLAYER_MENU].init(9,ptrArr+12);


}

void Menu::init(){
	_state = MAIN_MENU;

	setupDisplay();

	//_numpad.init();

	_itemTable[MAIN_MENU].push_back(&_logo);
	_itemTable[MAIN_MENU].push_back(&_play);
	//_itemTable[MAIN_MENU].push_back(&_server);
	//_itemTable[MAIN_MENU].push_back(&_client);
	_itemTable[MAIN_MENU].push_back(&_multiplayer);
	_itemTable[MAIN_MENU].push_back(&_tutorial);
	_itemTable[MAIN_MENU].push_back(&_credits);

	_itemTable[STAGE_MENU].push_back(&_stgThumb[0]);
	_itemTable[STAGE_MENU].push_back(&_stgThumb[1]);
	_itemTable[STAGE_MENU].push_back(&_stgThumb[2]);
	_itemTable[STAGE_MENU].push_back(&_stgThumb[3]);
	_itemTable[STAGE_MENU].push_back(&_stgNext);
	_itemTable[STAGE_MENU].push_back(&_stgPrev);

	_itemTable[MULTIPLAYER_MENU].push_back(&_mpServer);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpClient);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpPool);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpIp);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpLeft);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpRight);
	for(int i=0;i<10;++i)
		_itemTable[MULTIPLAYER_MENU].push_back(&_mpNumpad[i]);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpNext);
	_itemTable[MULTIPLAYER_MENU].push_back(&_mpPrev);

	_itemTable[TUTORIAL_MENU].push_back(&_next);
	_itemTable[TUTORIAL_MENU].push_back(&_prev);
	_itemTable[TUTORIAL_MENU].push_back(&_back);

	//for(int i=0;i<10;++i)
	//	_itemTable[MULTIPLAYER_MENU].push_back(&(_numpad._nums[i]));

	_logo.image_id = IMG_GFX::logo;

	_play.image_id = IMG_UI::play;
	_multiplayer.image_id = IMG_UI::server;
	//_server.image_id = IMG_UI::server;
	//_client.image_id = IMG_UI::client;
	_tutorial.image_id = IMG_UI::tutorial;
	_credits.image_id = IMG_UI::credits;

	_stgThumb[0].image_id = IMG_UI::stgthumb0;
	_stgThumb[1].image_id = IMG_UI::stgthumb0;
	_stgThumb[2].image_id = IMG_UI::stgthumb0;
	_stgThumb[3].image_id = IMG_UI::stgthumb0;
	_stgNext.image_id = IMG_UI::play;
	_stgPrev.image_id = IMG_UI::exit;

	_mpServer.image_id = IMG_UI::server;
	_mpClient.image_id = IMG_UI::client;
	_mpPool.image_id = IMG_UI::move;
	_mpIp.image_id = IMG_UI::move;
	_mpLeft.image_id = IMG_UI::move;
	_mpRight.image_id = IMG_UI::move;
	for(int i=0;i<10;++i)
		_mpNumpad[i].image_id = IMG_UI::num[i];
	_mpNext.image_id = IMG_UI::play;
	_mpPrev.image_id = IMG_UI::exit;

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
	int x,y,w,h;
	if(width > height){
		x = (width-height)/2;
		y = 0;
		w = height;
		h = height;
	}else{
		x = 0;
		y = (height-width)/2;
		w = width;
		h = width;
	}


	_display[MAIN_MENU].setGUI(x,y,w,h);
	_display[STAGE_MENU].setGUI(x,y,w,h);
	_display[MULTIPLAYER_MENU].setGUI(x,y,w,h);

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
			//_done = true;
			_state = STAGE_MENU;
		}else if(_tutorial.checkCollision(mx,my)){
			_state = TUTORIAL_MENU;
			_page = 0;
		}else if(_multiplayer.checkCollision(mx,my)){
			_state = MULTIPLAYER_MENU;
			/*if(_net == 0){
				_net = 1;
			}*/
		//}else if(_client.checkCollision(mx,my)){
			_state = MULTIPLAYER_MENU;
			/*if(_net == 0){
				_net = 2;
			}*/
		}
	}else if(_state == MULTIPLAYER_MENU){
		for(int i=0;i<10;++i){
			if(_mpNumpad[i].checkCollision(mx,my)){
				ipPushNum(i);
			}
		}

		if(_mpServer.checkCollision(mx,my)){
			if(_net == 0){
				_net = 1;
				uint8_t* v = (uint8_t*)(&_mpIpAddr);
				sscanf_s(_com->getLocalIp(),"%hhu.%hhu.%hhu.%hhu",v,v+1,v+2,v+3);
				int a =0;
			}
		}else if(_mpClient.checkCollision(mx,my)){
			if(_net == 0){
				_net = 2;
			}
		}else if(_mpNext.checkCollision(mx,my)){
			_state = STAGE_MENU;
		}else if(_mpPrev.checkCollision(mx,my)){
			_state = MAIN_MENU;
			_net = 0;
		}else if(_mpLeft.checkCollision(mx,my)){
			_mpIpPos = (_mpIpPos-1)%4;
		}else if(_mpRight.checkCollision(mx,my)){
			_mpIpPos = (_mpIpPos+1)%4;
		}
	}else if(_state == STAGE_MENU){
		if(_stgNext.checkCollision(mx,my)){
			_done = true;
		}
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
void Menu::render(renderer& ren, font& fon){
	for(int i=0;i<_itemTable[_state].size();++i){
		button* ptr = _itemTable[_state][i];
		ren.renderSprite(ptr->image_id,ptr->rect.x,ptr->rect.y,ptr->rect.w,ptr->rect.h);
	}
	if(_state == MULTIPLAYER_MENU){
		char buf[20];
		sprintf_s(buf,"%3d.%3d.%3d.%3d",(_mpIpAddr>>0)&0xFF,(_mpIpAddr>>8)&0xFF,(_mpIpAddr>>16)&0xFF,(_mpIpAddr>>24)&0xFF);
		ren.renderText(buf,fon,_mpIp.rect.x,_mpIp.rect.y);
		int xx = _mpIp.rect.x+(int)((float)_mpLeft.rect.w*(4.0f/3.0f))*_mpIpPos;
		ren.renderSprite(_mpLeft.image_id,xx,_mpIp.rect.y,_mpLeft.rect.w,_mpLeft.rect.h);
	}
}

bool Menu::isDone(){
	return _done;
}




