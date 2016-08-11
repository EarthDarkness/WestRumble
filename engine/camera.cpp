#include "camera.h"
#include <stdlib.h>

camera::camera(){
	_x = 0;
	_y = 0;
	_scale = 120;
}
camera::~camera(){
}

void camera::setPos(int x, int y){
	_x = x;
	_y = y;
}
void camera::addPos(int x, int y){
	_x += x;
	_y += y;
}
void camera::setScale(int scale){
	_scale = scale;
}
void camera::addScale(int scale){
	_scale += scale;
}

int camera::getX(){
	return _x;
}
int camera::getY(){
	return _y;
}
int camera::getScale(){
	return _scale;
}



camCtrl::camCtrl(){
	_x0 = _x = 0;
	_y0 = _y = 0;
	_time = 1.0f;

	_speed = 0.1f;

	_cam = NULL;
}
camCtrl::~camCtrl(){
}

void camCtrl::setCam(camera* cam){
	_cam = cam;
	_x0 = _x = _cam->getX();
	_y0 = _y = _cam->getY();
}
void camCtrl::setSpeed(float speed){
	_speed = speed;
	if(_speed >1.0f)
		_speed = 1.0f;
}

void camCtrl::goTo(int x, int y){
	_x0 = _cam->getX();
	_y0 = _cam->getY();
	_x = x;
	_y = y;
	_time = 0.0f;
}
void camCtrl::goFromTo(int x0, int y0, int x, int y){
	_x0 = x0;
	_y0 = y0;
	_x = x;
	_y = y;
	_time = 0.0f;
}

void camCtrl::update(){
	_time += _speed;
	if(_time > 1.0f)
		_time = 1.0f;
	float xx = (float)(_x-_x0)*_time+(float)_x0;
	float yy = (float)(_y-_y0)*_time+(float)_y0;
	_cam->setPos((int)xx,(int)yy);
}

bool camCtrl::isDone(){
	if(_time >= 1.0f)
		return true;
	return false;
}
