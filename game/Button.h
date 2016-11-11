#ifndef BOTTON_H
#define BOTTON_H
#include "panel.h"

#include <SDL.h>
#include <string>
#include <vector>
#include "../engine/animation.h"

using namespace std;


class button;
class clickHandler
{
private:
	int alloc();
public:
	int _time;
	vector<button*> _peers;

	clickHandler();
	~clickHandler();

	int getTime();

	void clickAt(int mx, int my, int time);

	void add(button* entity);
	void rem(button* entity);

};

static clickHandler BUTTONCLICKER;

class button : public guiEntry
{
public:
	animation _sprite;
	string _sound;
	SDL_Rect _rect;

	int _press;//time stamp


	button(); 
	~button();

	int getSprite();

	void setGUI(int x, int y, int w, int h);

	bool isPress();
	bool checkCollision(int mx, int my);

	bool isDone();

};


#endif