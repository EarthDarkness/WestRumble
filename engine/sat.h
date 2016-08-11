#ifndef SAT_H
#define SAT_H

#include "types.h"
#include <stdio.h>
#include <assert.h>

template <class T>
class sat
{
private:
	T* _data;
	u32 _size;
public:
	sat(){
		_size = 0;
		_data = NULL;
	}
	sat(u32 size){
		_size = 0;
		_data = NULL;
		if(size > 0){
			_size = size;
			_data = new T[_size];
		}
	}
	sat(const sat<T>& source){
		/*if(source.size() == 0){
			_size = 0;
			_data = NULL;
		}else{
			_size = source.size();
			_data = new T[_size];
			for(int i=0;i<_size;++i)
				_data[i] = source._data[i];
		}
		append(source);*/
		_size = 0;
		_data = NULL;
		clear();
		append(source);
	}
	~sat(){
		delete[] _data;
	}

	void init(u32 size){
		if(_data == NULL && size > 0){
			_size = size;
			_data = new T[_size];
		}
	}
	void init(T* data, u32 size){
		if(data == NULL || size == 0)
			return;
		clear();
		_data = data;
		_size = size;
	}
	void reset(u32 size){
		if(size > 0){
			if(_data != NULL)
				clear();
			init(size);
		}
	}
	void clear(){
		_size = 0;
		if(_data != NULL)
			delete[] _data;
		_data = NULL;
	}

	void fill(T data){
		for(int i=0;i<_size;++i)
			_data[i] = data;
	}
	void resize(u32 size){
		if(size > 0){
			T* ptr = new T[size];
			for(int i=0;i<size&&i<_size;++i)
				ptr[i] = _data[i];
			delete[] _data;
			_data = ptr;
			_size = size;
		}
	}
	void append(const sat<T>& source){
		if(source._size > 0){
			int size = _size;
			resize(_size+source._size);
			for(int i=size;i<_size;++i)
				_data[i] = source._data[i-size];
		}
	}
	void set(T item, u32 pos){
		if(_data == NULL || pos >= _size)
			return;
		_data[pos] = item;
	}
	T* getPtr(){
		return _data;
	}

	int size()const{
		return _size;
	}
	int size(){
		return static_cast<const sat<T>*>(this)->size();
	}
	bool empty()const{
		return _data == 0;
	}
	bool empty(){
		return static_cast<const sat<T>*>(this)->empty();
	}

	T& operator[](u32 pos)const{
		assert(pos < _size);
		assert(_data != NULL);
		return _data[pos];
	}
	T& operator[](u32 pos){
		return static_cast<const sat<T>*>(this)->operator[](pos);
	}
	T& at(u32 pos)const{
		assert(pos < _size);
		assert(_data != NULL);
		return _data[pos];
	}
	T& at(u32 pos){
		return static_cast<const sat<T>*>(this)->at(pos);
	}
};
#endif