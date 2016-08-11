//Movieclip Floating Point

#ifndef MOVIECLIPF_H
#define MOVIECLIPF_H

#include "sat.h"
#include <map>

using namespace std;

template <class T>
class movieClipF
{
private:
	sat<pair<T,bool> > _data;
	float _pos;
	int _current;
	float _speed;
	float _base;
	bool _paused;
	bool _done;
public:
	bool _loop;

	movieClipF(){
		_data.clear();
		_pos = 0.0f;
		_current = 0;
		_speed = 1.0f;
		_base = 1.0f;
		_paused = true;
		_done = false;
		_loop = false;
	}
	~movieClipF(){
		_data.clear();
	}

	void init(u32 size){
		_data.init(size);
		for(int i=0;i<size;++i){
			_data[i].second = false;
		}
	}
	void set(T data, u32 pos){
		if(pos >= _data.size())
			return;
		_data[pos].first = data;
		_data[pos].second = true;
	}

	int size(){
		return _data.size();
	}
	float getSpeed(){
		return _base;
	}

	void fill(){
		//need while
		//incomplete
		/*u32 begin = _data._size;
		u32 end = _data._size;
		for(int i=0;i<_data._size;++i)
			if(_data._data[i].second)
				begin = i;

		if(begin == _data._size)
			return;

		for(int i=begin+1;i<_data._size;++i)
			if(_data._data[i].second)
				end = i;

		if(end == _data._size){
			for(int i=0;i<_data._size;++i){
				if(i != begin){
					_data._data[i].first = _data._data[begin].first;
				}
			}
			return;
		}
		u32 delta = end-begin;
		T dif = _data._data[end].first - _data._data[begin].first * (1.0f/(float)delta);
		for(int i=1;i<delta;++i){
			_data._data[begin+i].first = dif*(float)i;
		}*/

		u32 num = 0;
		u32 a=_data.size();
		u32 b=_data.size();
		for(int i=0;i<_data.size();++i){
			if(_data[i].second){
				++num;
				if(i<a)
					a=i;
				if(i!=a && i<b)
					b=i;
			}
		}
		if(num == 0)//no data
			return;
		if(num == 1){//no interpolation
			for(int i=0;i<_data.size();++i){
				if(i != a){
					_data[i].first = _data[a].first;
					_data[i].second = true;
				}
			}
			return;
		}
		//num-=1;
		for(int i=0;i<num;++i){
			u32 s = (_data.size() +b -a)%_data.size();

			T dif = (_data[b].first - _data[a].first)*(1.0f/(float)s);
			if(b<a)
				dif = -dif;
			for(int j=1;j<s;++j){
				if(b<a)
					_data[(a+j)%_data.size()].first = _data[b].first+dif*(float)(s-j); 
				else
					_data[(a+j)%_data.size()].first = _data[a].first+dif*(float)j;
				_data[(a+j)%_data.size()].second = true;
			}

			if(i < num-1){
				a=b;
				for(int j=0;j<_data.size();++j){
					if(_data[(j+1+b)%_data.size()].second){
						b=(j+1+b)%_data.size();
						break;
					}
				}
			}
		}
		

	}

	void play(){
		_paused = false;
		if(_pos==0 || _pos==_data.size()-1)
			_done = false;
	}
	void pause(){
		_paused = true;
	}
	void stop(){
		_pos = 0;
		_paused = true;
		_done = false;
	}

	void goTo(u32 pos){
		if(pos < _data.size())
			_pos = pos;
	}
	void setSpeed(float speed){
		_base = speed;
		_speed = 1.0f;
	}

	void forward(){
		if(_speed > 0.0f)
			_speed *= 2.0f;
		else
			_speed = 1.0f;
	}
	void backward(){
		if(_speed < 0.0f)
			_speed *= 2.0f;
		else
			_speed = -1.0f;
	}

	void update(){
		_current = (int)_pos;
		if(!_paused){
			_pos += _speed*_base;
			if((int)_pos >= 0 && (int)_pos < _data.size())
				return;
			if(_loop){
				_pos = (float)(((int)_pos+_data.size())%_data.size());
			}else{
				_done = true;
				if((int)_pos < 0){
					_pos = 0.0f;
				}else if(_pos >= _data.size()){
					_pos = (float)(_data.size()-1);
				}
			}
		}
	}

	u32 getCurrent(){
		return _current;
	}
	bool isDone(){
		return _done;
	}

	T& get(u32 pos){
		return _data[pos].first;
	}
	T& get(){
		return _data[_current].first;
	}

};



#endif