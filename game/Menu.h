#ifndef MENU_H
#define MENU_H

#include "../engine/input.h"
#include <SDL.h>
#include <string>
#include "../engine/resources.h"
#include "../engine/renderer.h"

#include "Button.h"
#include "Actors.h"

enum MENU_STATES{
	MAIN_MENU = 0,
	STAGE_MENU,
	TUTORIAL_MENU,
	CREDITS_MENU,
	MAX_MENU
};

class Menu
{
private:
	input* _input;

public:
	Menu();
	~Menu();

	void init();

	void setGUI(int width, int height);

	void udpdate(int mx, int my);
	void render(renderer& ren);

	bool isDone();

	bool _done;
	int _net;

	//mainmenu
	string _bgn;
	
	Button _logo;

	Button _play;
	Button _server;
	Button _client;
	Button _tutorial;
	Button _credits;
	Button _exit;

	//tutorial
	int _page;
	Button _next;
	Button _prev;
	Button _back;

	Button _info[4];



	int _state;
	//Button stages[4];

	vector<Button*> _itemTable[MAX_MENU];


};

#endif
