



#ifndef MOVIECLIPF_H
#define MOVIECLIPF_H

class movieClipW
{
private:
	int _size;		//numebr of frames
	float _pos;		//current floating time
	int _current;	//current sprite 
	float _speed;	//time step multiplyer
	float _base;	//base time step
	bool _paused;	//is paused
	bool _done;		//animation finished
public:
	bool _loop;		//aniamtion loops


	movieClipW() {
		_size = 1;
		_pos = 0.0f;
		_current = 0;
		_speed = 1.0f;
		_base = 1.0f;
		_paused = true;
		_done = false;
		_loop = false;
	}
	~movieClipW() {
	}

	void init(int size) {
		_size = size;
	}

	int size() {
		return _size;
	}
	float getSpeed() {
		return _base;
	}

	void play() {
		_paused = false;
		if (_pos == 0 || _pos == _size - 1)
			_done = false;
	}
	void pause() {
		_paused = true;
	}
	void stop() {
		_pos = 0;
		_paused = true;
		_done = false;
	}

	void goTo(int pos) {
		if (pos < _size)
			_pos = pos;
	}
	void setSpeed(float speed) {
		_base = speed;
		_speed = 1.0f;
	}

	void forward() {
		if (_speed > 0.0f)
			_speed *= 2.0f;
		else
			_speed = 1.0f;
	}
	void backward() {
		if (_speed < 0.0f)
			_speed *= 2.0f;
		else
			_speed = -1.0f;
	}

	void update() {
		_current = (int)_pos;
		if (!_paused) {
			_pos += _speed*_base;
			if ((int)_pos >= 0 && (int)_pos < _size)
				return;
			if (_loop) {
				_pos = (float)(((int)_pos + _size) % _size) + (_pos-(float)(_size));
			}
			else {
				_done = true;
				if ((int)_pos < 0) {
					_pos = 0.0f;
				}
				else if (_pos >= _size) {
					_pos = (float)(_size - 1);
				}
			}
		}
	}

	int get() {
		return _current;
	}
	bool isDone() {
		return _done;
	}

};

#endif