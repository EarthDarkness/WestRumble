#ifndef MATRIXMN_H
#define MATRIXMN_H

#include <assert.h>
#include <stdlib.h>

template <class T>
class matMN
{
private:
	T* _data;
	int _width;
	int _height;
public:
	matMN(){
		_width = 0;
		_height = 0;
		_data = NULL;
	}
	matMN(int width, int height){
		_width = 0;
		_height = 0;
		_data = NULL;
		if(width > 0 && height > 0){
			_width = width;
			_height = height;
			_data = new T[_width*_height];
		}
	}
	matMN(const matMN<T>& source){
		_width = 0;
		_height = 0;
		_data = NULL;
		if(source._width > 0 && source._height > 0){
			init(source._width, source._height);
			int size = _width*_height;
			for(int i=0;i<size;++i)
				_data[i] = source._data[i];
		}
	}
	~matMN(){
		delete[] _data;
	}

	void init(int width, int height){
		if(width > 0 && height > 0){
			clear();
			_width = width;
			_height = height;
			_data = new T[_width*_height];
		}
	}
	void init(T* data, int width, int height){
		if(data == NULL || width <= 0 || height <= 0)
			return;
		clear();
		_width = width;
		_height = height;
		_data = data;
	}

	void clear(){
		_width = 0;
		_height = 0;
		if(_data != NULL)
			delete[] _data;
		_data = NULL;
	}

	void fill(T data){
		if(_data == NULL)
			return;
		int size = _width * _height;
		for(int i=0;i<size;++i)
			_data[i] = data;
	}

	void set(T item, int pos){
		if(_data == NULL || pos >= _width*_height)
			return;
		_data[pos] = item;
	}
	void set(T item, int x, int y){
		if(_data == NULL || x < 0 || x >= _width || y < 0 || y >= _height)
			return;
		_data[y*_width+x] = item;
	}
	T* getPtr(){
		return _data;
	}

	int width()const{
		return _width;
	}
	int height()const{
		return _height;
	}
	int width(){
		return static_cast<const matMN<T>*>(this)->width();
	}
	int height(){
		return static_cast<const matMN<T>*>(this)->height();
	}
	bool empty()const{
		return _data == NULL;
	}
	bool empty(){
		return static_cast<const matMN<T>*>(this)->empty();
	}

	T& operator[](int pos)const{
		assert(pos < _width*_height);
		assert(_data != NULL);
		return _data[pos];
	}
	T& operator[](int pos){
		return static_cast<const matMN<T>*>(this)->operator[](pos);
	}
	T& at(int pos)const{
		assert(pos < _width*_height);
		assert(pos >= 0);
		assert(_data != NULL);
		return _data[pos];
	}
	T& at(int pos){
		return static_cast<const matMN<T>*>(this)->at(pos);
	}
	T& at(int x, int y)const{
		assert(x < _width);
		assert(y < _height);
		assert(x >= 0);
		assert(y >= 0);
		assert(_data != NULL);
		return _data[y*_width+x];
	}
	T& at(int x, int y){
		return static_cast<const matMN<T>*>(this)->at(x,y);
	}
};






#endif