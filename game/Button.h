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

	void clickAt(int mx, int my);
	void clickAt();

	void add(button* entity);
	void rem(button* entity);

};

extern clickHandler BUTTONCLICKER;

class button : public guiEntry
{
	friend class clickHandler;
private:
public:
	bool checkCollision(int mx, int my);
	animation _sprite;
	string _sound;
	SDL_Rect _rect;

	int _press;//time stamp


	button(); 
	~button();

	int getSprite();

	void setGUI(int x, int y, int w, int h);

	bool isPress();

	bool isDone();

};


#endif