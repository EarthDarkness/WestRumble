#ifndef STORE_H
#define STORE_H

#include <map>

#include "Team.h"
#include "Button.h"
#include "../engine/core.h"

struct goodsData
{
	int _price;
	int _stock;
	button _gui;
	PowerUp _data;

	goodsData();
	~goodsData();

	void set(int price, int powup, int stock);
};

class store
{
private:
	Team* _client;

	int _x;
	int _y;

	goodsData _goods[9];
	button _exit;
	button _player[5];

	int _selected;
	int _done;
public:
	store();
	~store();

	void setGUI(int width, int height);
	void load(resources& res);

	void init(Team* client, int color);
	void update(core& engine);
	void render(core& engine);

	bool isDone();

};

#endif
