#ifndef MENU_H
#define MENU_H

#include "../engine/input.h"
#include <SDL.h>
#include <string>
#include "../engine/resources.h"
#include "../engine/renderer.h"

#include "Button.h"
#include "Actors.h"
#include "Props.h"

//class numpad
//{
//public:
//	button _nums[10];
//	numpad(){
//	}
//	~numpad(){
//	}
//
//	void init(){
//		for(int i=0;i<10;++i)
//			_nums[i].image_id = IMG_UI::num[i];
//	}
//
//	void setCoords(int x, int y, int w, int h){
//		int wsp = ((float)w*0.4f);
//		int hsp = ((float)w*0.4f);
//
//		int wn = w-wsp;
//		int hn = h-hsp;
//
//		int wc = wn/3;
//		int hc = hn/4;
//
//		for(int j=0;j<3;++j){
//			for(int i=0;i<3;++i){
//				int xx = x+i*(wsp/2+wc);
//				int yy = y+j*(hsp/3+hc);
//
//				_nums[1+j*3+i].setPos(xx,yy,wc,hc);
//			}
//		}
//		_nums[0].setPos(x+1*(wsp/2+wc),y+3*(hsp/3+hc),wc,hc);
//
//	}
//	int getNum(int mx, int my){
//		for(int i=0;i<10;++i){
//			if(_nums[i].checkCollision(mx,my)){
//				return i;
//			}
//		}
//		return -1;
//	}
//
//};

enum MENU_STATES{
	MAIN_MENU = 0,
	STAGE_MENU,
	TUTORIAL_MENU,
	CREDITS_MENU,
	MULTIPLAYER_MENU,
	MAX_MENU
};

class Menu
{
private:
	input* _input;

	//panel _display[MAX_MENU];
	board _display[MAX_MENU];

public:
	Menu();
	~Menu();

	void setupDisplay();
	void init();

	void setGUI(int width, int height);

	void udpdate(int mx, int my);
	void render(renderer& ren);

	bool isDone();

	bool _done;
	int _net;

	//mainmenu
	string _bgn;
	
	button _logo;

	button _play;
	button _server;
	button _client;
	button _tutorial;
	button _credits;
	button _exit;

	//stage select
	button _stgThumb[4];
	button _stgNext;
	button _stgPrev;

	//tutorial
	int _page;
	button _next;
	button _prev;
	button _back;

	button _info[4];

	//multiplayer
	button _mpServer;
	button _mpClient;
	button _mpIp;
	button _mpLeft;
	button _mpRight;
	button _mpPool;
	button _mpNumpad[10];
	button _mpNext;
	button _mpPrev;


	int _state;
	//Button stages[4];

	vector<button*> _itemTable[MAX_MENU];


};

#endif
