#include "Button.h"
#include "Props.h"

int clickHandler::alloc(){
	for(int i=0;i<_peers.size();++i)
		if(_peers[i] == NULL)
			return i;
	_peers.push_back(NULL);
	return _peers.size()-1;
}

clickHandler::clickHandler(){
	_time = 0;
}
clickHandler::~clickHandler(){
}

int clickHandler::getTime(){
	return _time;
}

void clickHandler::clickAt(int mx, int my){
	++_time;
	for(int i=0;i<_peers.size();++i){
		if(_peers[i] != NULL)
		if(_peers[i]->checkCollision(mx,my)){
			_peers[i]->_press = _time;
		}
	}
}
void clickHandler::clickAt(){
	++_time;
}

void clickHandler::add(button* entity){
	_peers[alloc()] = entity;
}

void clickHandler::rem(button* entity){
	for(int i=0;i<_peers.size();++i)
		if(_peers[i] == entity)
			_peers[i] = NULL;
}

clickHandler BUTTONCLICKER;



button::button(){
	_sound = "";
	_press = -1;

	BUTTONCLICKER.add(this);	
}
button::~button(){
	BUTTONCLICKER.rem(this);	
}

int button::getSprite(){
	return G_Anim[_sprite.get()];
}

void button::setGUI(int x, int y, int w, int h){
	_rect.x = x;
	_rect.y = y;
	_rect.w = w;
	_rect.h = h;
}

bool button::isPress(){
	return _press == BUTTONCLICKER.getTime();

}

bool button::checkCollision(int mx, int my){
	return 
		mx >= _rect.x &&
		mx < _rect.x+_rect.w &&
		my >= _rect.y &&
		my < _rect.y+_rect.h 
		;
}

bool button::isDone(){
	return _sprite.isDone();
}
