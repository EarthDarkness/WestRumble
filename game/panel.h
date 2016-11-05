#ifndef PANEL_H
#define PANEL_H

#include "..\engine\matrixMN.h"
#include <vector>

class guiEntry
{
public:
	virtual void setGUI(int x, int y, int w, int h) = 0;
	virtual bool doDestroy(){
		return false;
	}
};


class panel : public guiEntry
{
	float _wSpacing;//percent of empty space between elements
	float _hSpacing;
	matMN<guiEntry*> _elements;

public:
	panel();
	panel(int width, int height, float wSpacing, float hSpacing, guiEntry** members);
	~panel();

	void init(int width, int height, float wSpacing, float hSpacing, guiEntry** members);

	void setGUI(int x, int y, int w, int h);
	bool doDestroy();
};


#endif


