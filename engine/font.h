#ifndef FONT_H
#define FONT_H

#include "resources.h"

class font
{
private:
	int _imgs[128];
	int _width;
	int _height;

public:
	font();
	~font();

	void load(const char* file, resources& res);
	void clear(resources& res);//call before destruction or resource leak
	
	int getChar(char c);
	int width();
	int height();
	

};

#endif