#ifndef MENU_H
#define MENU_H

#include "../engine/input.h"
#include <SDL.h>
#include <string>
#include "../engine/resources.h"
#include "../engine/renderer.h"
#include "../engine/network.h"
#include "../engine/font.h"

#include "Button.h"
#include "Actors.h"
#include "Props.h"

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

	board _display[MAX_MENU];

	void ipPushNum(int num);

public:
	network* _com;//TEMP

	Menu();
	~Menu();

	void setupDisplay();
	void init();

	void setGUI(int width, int height);

	void reset();

	void udpdate(int mx, int my);
	void render(renderer& ren, font& fon);

	bool isDone();

	bool _done;
	int _net;

	//mainmenu
	string _bgn;
	
	button _logo;

	button _play;
	button _multiplayer;
	button _tutorial;
	button _credits;
	button _exit;

	//stage select
	int _map;
	button _stgSelect;
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
	button _mpSelect;
	button _mpIp[4];
	button _mpLeft;
	button _mpRight;
	button _mpPool;
	button _mpNumpad[10];
	button _mpNext;
	button _mpPrev;

	int _mpIpPos;
	int _mpIpAddr;


	int _state;
	//Button stages[4];

	vector<button*> _itemTable[MAX_MENU];


};

#endif
