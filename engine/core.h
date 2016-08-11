#ifndef CORE_H
#define CORE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "input.h"
#include "resources.h"
#include "renderer.h"


class core
{
private:
	SDL_Event _event;
	bool _running;

	SDL_Window* _window;

public:
	resources _res;
	renderer _render;
	input _input;
	font _font;

	bool _winChanged;
	bool _back;

	//int _time;//nevermind using SDL VSYNC

	core();
	~core();

	int init();
	int createWindow(const char* name, int w, int h);
	void loadFont();
	void clear();

	SDL_Window* getWindow();


	void sortEvents();
	void flush();

	bool isRunning();
	void quit();

};


#endif