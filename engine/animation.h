#ifndef ANIM_H
#define ANIM_H

#include "movieClipW.h"
#include "resources.h"
#include <map>

using namespace std;
static const int MAX_ANIMS = 32;


class animation
{
private:
	int _index[MAX_ANIMS];//pointer** (index) to start of aniamtion
	movieClipW _frame[MAX_ANIMS];//frame of aniamtion (index+frame == image id)

	int _state;//enum
public:
	animation();
	~animation();

	void init(animation& src);

	int get();

	bool isDone();//is done current anim

	void setState(int stt);

	int getState();

	void loadAnim(int stt, int index, int size, float speed, bool loop);
};
#endif