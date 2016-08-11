#include "font.h"
#include "translator.h"
#include <sstream>

using namespace std;

font::font(){
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
	string folder = "resources/font/";

	while(!in.eof()){
		char ch;
		string str;
		in >> ch;
		in >> str;

		_imgs[ch].assign(str);
		//_imgs[ch].assign("L1");

		string path = folder + str;
		res.loadImage(path.c_str(),str.c_str());
	}

	//in.close();

	/*_width = 24;
	_height = 32;

	_imgs['0'].assign("0.png");
	_imgs['1'].assign("1.png");
	_imgs['2'].assign("2.png");
	_imgs['3'].assign("3.png");
	_imgs['4'].assign("4.png");
	_imgs['5'].assign("5.png");
	_imgs['6'].assign("6.png");
	_imgs['7'].assign("7.png");
	_imgs['8'].assign("8.png");
	_imgs['9'].assign("9.png");

	res.loadImage("resources/font/0.png","0.png");
	res.loadImage("resources/font/1.png","1.png");
	res.loadImage("resources/font/2.png","2.png");
	res.loadImage("resources/font/3.png","3.png");
	res.loadImage("resources/font/4.png","4.png");
	res.loadImage("resources/font/5.png","5.png");
	res.loadImage("resources/font/6.png","6.png");
	res.loadImage("resources/font/7.png","7.png");
	res.loadImage("resources/font/8.png","8.png");
	res.loadImage("resources/font/9.png","9.png");*/


}
void font::clear(resources& res){
	for(int i=0;i<128;++i){
		if(_imgs[i].empty())
			continue;
		res.eraseImage(_imgs[i].c_str());
		_imgs[i].clear();
	}
}

const char* font::getChar(char c){
	if(_imgs[c].empty())
		return NULL;
	return _imgs[c].c_str();
}
int font::width(){
	return _width;
}
int font::height(){
	return _height;
}

