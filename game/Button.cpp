#include "Button.h"
Button::Button(){
	image_id = 0;
	image_press_id = 0;
	sound_press_id = 0;
}
Button::~Button(){

}

void Button::setPos(int x, int y, int w, int h){
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

bool Button::checkCollision( int x, int y)
{
	return static_cast<const Button*>(this)->checkCollision(x,y);
}
bool Button::checkCollision( int x, int y)const
{
	if (x > (rect.x) && x < (rect.x + rect.w)){
		if (y >(rect.y) && y < (rect.y + rect.h)){
			return true;
		}
	}
	return false;
}