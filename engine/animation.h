#ifndef ANIM_H
#define ANIM_H

#include "movieClipW.h"
#include "resources.h"
#include <map>

using namespace std;


class animation
{
private:
	//map<string,movieClipF<string> > _data;
	int _index[50];//50 = max aniamtions//pointer** (index) to start of aniamtion
	movieClipW _frame[50];//up//frame of aniamtion (index+frame == image id)

	string _state;//to int//enum
public:
	animation();
	~animation();

	void init(animation& src);

	void reset(const char* name);
	const char* get(const char* name);//update and return proper iamge of animation
	const char* get();
	bool isDone(const char* name);

	void set(const char* anim, const char* name);
	void setState(string state);

	const char* getState();

	map<string,movieClipF<string> >& getData();

	void loadAnim(const char* file);
	static void loadRes(const char* file, resources& res);
};
#endif