#ifndef MENU_H
#define MENU_H

#include "../engine/input.h"
#include <SDL.h>
#include <string>
#include "../engine/resources.h"
#include "Button.h"
#include "Actors.h"

using namespace std;

enum MENUENTRY{
	MAINMENU = 0,
	STAGEMENU,
	TUTORIALMENU,
	CREDITSMENU
};

class Menu
{

private:
	input* _input;
public:
	Menu();
	~Menu();

	void setGUI(int width, int height);

	//mainmenu
	string _bgn;
	
	Button _logo;

	Button _play;
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



};

#endif
