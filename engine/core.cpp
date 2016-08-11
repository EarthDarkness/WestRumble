//#ver

#include "core.h"
#include <iostream>

using namespace std;


SDL_Renderer* engRenderer = NULL;

core::core(){
	_window = NULL;
	engRenderer = NULL;
	_running = true;
	_winChanged = false;
	_back = false;
}
core::~core(){
	clear();
}

int core::init(){
	int error;
	int flags;
	_render.init(&_res);
	error = SDL_Init(SDL_INIT_EVENTS|SDL_INIT_TIMER|SDL_INIT_VIDEO);
	if(error < 0){
		cerr << "Error initializing SDL2, with code: " << error << endl;
		return error;
	}
	flags = IMG_INIT_PNG|IMG_INIT_JPG;
	error = IMG_Init(flags);
	if(error != flags){
		cerr << "Error initializing SDL2_image, with flags: " << error << endl;
		return -1;
	}
	flags = 0;
	error = Mix_Init(flags);
	if(error != flags){
		cerr << "Error initializing SDL2_mixer, with flags: " << error << endl;
		return -1;
	}
	error = Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
	if(error != 0){
		cerr << "Error opening audio mixer." << endl;
		return -1;
	}
	return 0;
}
int core::createWindow(const char* name, int w, int h){
	int error = 0;
	if(_window != NULL){
		SDL_DestroyWindow(_window);
		error = 1;
	}
	_window = SDL_CreateWindow(name,100,100,w,h,SDL_WINDOW_SHOWN|SDL_WINDOW_MAXIMIZED|SDL_WINDOW_RESIZABLE);//|SDL_WINDOW_FULLSCREEN);//
	if(engRenderer != NULL){
		SDL_DestroyRenderer(engRenderer);
		error += 2;
	}
	engRenderer = SDL_CreateRenderer(_window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	return error;
}
void core::loadFont(){
	_font.load("resources/font/font.txt",_res);
}
void core::clear(){
	_font.clear(_res);
	SDL_DestroyRenderer(engRenderer);
	SDL_DestroyWindow(_window);
	Mix_CloseAudio();
	Mix_Quit();//Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();
}

SDL_Window* core::getWindow(){
	return _window;
}

void core::sortEvents(){
	while(SDL_PollEvent(&_event)){
		if(_event.type == SDL_QUIT || _event.type == SDL_APP_TERMINATING)
			_running = false;	
		if(_event.type == SDL_KEYDOWN)
			if(_event.key.keysym.sym == SDLK_AC_BACK)
				_back = true;//_running = false;
		/*if(_event.type == SDL_MOUSEMOTION){
			_input.update(true,_event.motion.x,_event.motion.y);
		}*/
		if(_event.type == SDL_MOUSEBUTTONDOWN){
			if(_event.button.button == 1)
				_input.update(true,_event.motion.x,_event.motion.y);
		}else if(_event.type == SDL_MOUSEBUTTONUP){
			if(_event.button.button == 1)
				_input.update(false,_event.motion.x,_event.motion.y);
		}
		if(_event.type == SDL_FINGERDOWN){
			int ww,hh;
			SDL_GetWindowSize(_window,&ww,&hh);
			_input.update(true,_event.tfinger.x*(float)ww,_event.tfinger.y*(float)hh);
		}else if(_event.type == SDL_FINGERUP){
			int ww,hh;
			SDL_GetWindowSize(_window,&ww,&hh);
			_input.update(false,_event.tfinger.x*(float)ww,_event.tfinger.y*(float)hh);
		}
		if(_event.type == SDL_WINDOWEVENT)
			if(_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				_winChanged = true;
	}
	SDL_RenderClear(engRenderer);
}
void core::flush(){
	SDL_RenderPresent(engRenderer);
}

bool core::isRunning(){
	return _running;
}
void core::quit(){
	_running = false;
}

