#ifndef INPUT_H
#define INPUT_H

#include "types.h"

class input
{
private:
	u8 _state;

	s32 _xPos;
	s32 _yPos;

public:
	input();
	~input();

	void update(bool down, s32 x, s32 y);

	bool isPress();
	bool isUp();
	bool isDown();

	s32 getX();
	s32 getY();

	void reset();
};

#endif