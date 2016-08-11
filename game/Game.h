#ifndef GAME_H
#define GAME_H

#include "Menu.h"
#include "Stage.h"
#include "../engine/core.h"
#include "../engine/translator.h"
#include "Button.h"
#include "Character.h"
#include "bomb.h"
#include "store.h"
#include "enum.h"

#include <stack>
#include <queue>

#include <iostream>

using namespace std;

class Game
{
private:

	const char* title;
	int width_screen;
	int height_screen;

	Menu menu;
	store shop;
	Stage stage;
	int state;
	Actors *selected;
	Actors *visualized;
	
	Actors *special;

	//movieClipF<string>* _waitForAnim;
	//movieClipF<
	//bool _animDone;

	

	queue<int> _selectedActions;

	int player;
	bool field;

	bool endGame;
	int winner;

	void centerAt(Actors* entry);
	void centerTeam(int t);

	void checkEnd();

	void calcActions();
	void initAction();
	void updateAction(int x, int y);

	void turnPlayer();
	void turnField();

	void updateMenu();
	void updateShop();
	void updateStage();
	void updateEnd();

	void renderMenu();
	void renderShop();
	void renderStage();
	void renderEnd();

	Team A;
	Team B;

	Button button_move_right;
	Button button_move_left;
	Button button_move_up;
	Button button_move_down;

	Button button_end_turn;

	//status
	string _speed;
	string _bombs;
	string _fire;
	string _shield;
	string _turn;


public:
	Game();
	~Game();

	core engine;
	camCtrl _cam;


	void init();
	void setGUI();

	void loadResources();

	void reset();
	void update();
	void render();


};

#endif
