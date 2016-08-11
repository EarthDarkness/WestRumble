#include "animation.h"
#include "translator.h"

#include <sstream>
#include <vector>

using namespace std;

animation::animation(){
	_state = "default";
	_data["default"].init(1);
	_data["default"].set("def.png",0);
}
animation::~animation(){
}


void animation::init(animation& src){
	if(!_data.empty())
		_data.clear();
	for(map<string,movieClipF<string> >::iterator it = src.getData().begin();it != src.getData().end();++it){
		_data[it->first].init(it->second.size());
		_data[it->first]._loop = it->second._loop;

		_data[it->first].setSpeed(it->second.getSpeed());

		for(int i=0;i<it->second.size();++i){
			_data[it->first].set(it->second.get(i),i);
		}
	}
}

void animation::reset(const char* name){
	if(_data.find(name) == _data.end())
		return;
	_data[name].goTo(0);
	_data[name].play();
}
const char* animation::get(const char* name){
	if(_data.find(name) == _data.end())
		return NULL;
	_data[name].update();
	return _data[name].get().c_str();//get proper animation
}
const char* animation::get(){
	if(_data.find(_state.c_str()) == _data.end())
		return NULL;
	_data[_state.c_str()].update();
	return _data[_state.c_str()].get().c_str();//get proper animation
}
bool animation::isDone(const char* name){
	if(_data.find(name) == _data.end())
		return true;
	return _data[name].isDone();
}

void animation::set(const char* anim, const char* name){
	_data[anim].init(1);
	_data[anim].set(name,0);
}
void animation::setState(string state){
	_data[_state].stop();
	_state = state;
	_data[_state].play();
}

const char* animation::getState(){
	return _state.c_str();
}

map<string,movieClipF<string> >& animation::getData(){
	return _data;
}

void animation::loadAnim(const char* file){
	stringstream in;
	vector<string> data;

	readRW(in,file);

	char ch;
	string str;
	int id;
	int size;
	float speed;

	in >> size;
	for(int i=0;i<size;++i){
		str.clear();
		in >> str;
		data.push_back(str);
	}

	while(!in.eof()){
		str.clear();
		in >> str;
		in >> size;
		in >> speed;
		in >> ch;

		_data[str.c_str()].init(size);
		if(ch == 'L')
			_data[str.c_str()]._loop = true;
		else if(ch == 'S')
			_data[str.c_str()]._loop = false;

		_data[str.c_str()].setSpeed(speed);

		for(int i=0;i<size;++i){
			in >> id;
			_data[str.c_str()].set(data[id],i);
		}
		//_data[str.c_str()].play();
	}
}

void animation::loadRes(const char* file, resources& res){
	stringstream in;
	vector<string> data;

	readRW(in,file);

	string str;
	int size;

	in >> size;
	for(int i=0;i<size;++i){
		str.clear();
		in >> str;
		data.push_back(str);
	}

	for(int i=0;i<data.size();++i){
		string imgpath = "resources/image/";
		imgpath += data[i];
		res.loadImage(imgpath.c_str(),data[i].c_str());
	}
}



