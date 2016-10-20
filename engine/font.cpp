#include "font.h"
#include "translator.h"
#include <sstream>
#include <memory.h>

using namespace std;

font::font(){
	memset(_imgs,0,sizeof(_imgs));
	_width = 0;
	_height = 0;
}
font::~font(){
}

void font::load(const char* file, resources& res){
	stringstream in;

	//in.open(file,ios::in);
	//if(!in)
	//	return;

	readRW(in,file);

	in >> _width;
	in >> _height;

	const char* folder = "resources/font/";

	while(!in.eof()){
		char ch;
		string str;

		in >> ch;
		in >> str;

		string path = folder + str;

		path.assign(folder);
		path.append(str);

		_imgs[ch]= res.loadImage(path.c_str());

	}
}
void font::clear(resources& res){
	memset(_imgs,0,sizeof(_imgs));
}

int font::getChar(char c){
	return _imgs[c];
}
int font::width(){
	return _width;
}
int font::height(){
	return _height;
}

