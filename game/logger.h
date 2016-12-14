#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

#include "TileMap.h"
#include "Props.h"

#include "..\engine\animation.h"


class field
{
public:
	matMN<int> _map;
	animation _sprite;

	field(){
	}
	~field(){
	}

	void init(TileMap& tm){
		_map.init(tm._map.width(),tm._map.height());

		for(int i=0;i<16;++i){
			_sprite.loadAnim(i,IMG_UI::heat[i],1,1.0f,false);
		}

		clear();
	}
	void clear(){
		_map.fill(0);
	}

	void add(int x, int y, int val){
		if(_map.empty())
			return;

		_map.at(x,y) += val;

	}
	void set(int x, int y, int val){
		if(_map.empty())
			return;

		_map.at(x,y) = val;

	}

	void load(const char* file){
		ifstream in;
		in.open(file,ios::in);

		if(!in)
			return;

		int w,h;

		in >> w;
		in >> h;

		if(_map.empty() || _map.width() != w || _map.height() != h){
			_map.init(w,h);
			_map.fill(0);
		}

		for(int j=0;j<_map.height();++j){
			for(int i=0;i<_map.width();++i){
				int val;
				in >> val;
				_map.at(i,j) += val;
			}
		}


		in.close();
	}
	void save(const char* file){
		if(_map.empty())
			return;

		ofstream out;
		out.open(file,ios::out);

		if(!out)
			return;

		out << _map.width() << endl;
		out << _map.height() << endl;

		for(int j=0;j<_map.height();++j){
			for(int i=0;i<_map.width();++i){

				out << _map.at(i,j) << " ";
			}
			out << endl;
		}


		out.close();

	}

	int sample(int x, int y){
		if(x >= _map.width() || x < 0)
			return 0;
		if(y >= _map.height() || y < 0)
			return 0;
		return _map.at(x,y);
	}

	void mult(int val){
		for(int j=0;j<_map.height();++j){
			for(int i=0;i<_map.width();++i){
				_map.at(i,j) *= val;
			}
		}

	}
	void div(int val){
		for(int j=0;j<_map.height();++j){
			for(int i=0;i<_map.width();++i){
				_map.at(i,j) /= val;
			}
		}

	}

	void smooth(int step){
		matMN<int> buf;
		buf.init(_map.width(),_map.height());
		buf.fill(0);

		for(int k=0;k<step;++k){
			for(int j=0;j<_map.height();++j){
				for(int i=0;i<_map.width();++i){
					int acc = 0;
					acc += 3*sample(i,j);
					acc += 2*sample(i+1,j);
					acc += 2*sample(i-1,j);
					acc += 2*sample(i,j+1);
					acc += 2*sample(i,j-1);
					acc += 1*sample(i-1,j-1);
					acc += 1*sample(i-1,j+1);
					acc += 1*sample(i+1,j-1);
					acc += 1*sample(i+1,j+1);
					int val = acc / 17;
					if(val == 0 && acc%17 > 11)
						++val;

					buf.at(i,j) = val;
				}
			}

			for(int j=0;j<_map.height();++j){
				for(int i=0;i<_map.width();++i){
					_map.at(i,j) = buf.at(i,j);
				}
			}
		}
	}

};


extern field _log;
extern string _logFile;




#endif
