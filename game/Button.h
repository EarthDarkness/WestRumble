#ifndef BOTTON_H
#define BOTTON_H
#include "panel.h"

#include <SDL.h>
#include <string>
using namespace std;

class button : public guiEntry
{
public:
	int image_id;
	int image_press_id;
	int sound_press_id;
	SDL_Rect rect;

	button(); 
	~button();

	void setGUI(int x, int y, int w, int h);

	bool checkCollision(int x, int y);
	bool checkCollision(int x, int y)const;

};


#endif