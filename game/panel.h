#ifndef PANEL_H
#define PANEL_H

#include "..\engine\matrixMN.h"
#include "..\engine\sat.h"
#include <vector>

class guiEntry
{
public:
	float _xrel = 0.0f;
	float _yrel = 0.0f;
	float _wrel = 0.0f;
	float _hrel = 0.0f;


	void init(float xr, float yr, float wr, float hr, bool center = false){
		if(center){
			_xrel = xr-wr*0.5f;
			_yrel = yr-hr*0.5f;
			_wrel = wr;
			_hrel = hr;
		}else{
			_xrel = xr;
			_yrel = yr;
			_wrel = wr;
			_hrel = hr;
		}
	}

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

class board : public guiEntry
{
	sat<guiEntry*> _elements;

public:
	board();
	board(int size, guiEntry** members);
	~board();

	void init(int size, guiEntry** members);

	void setGUI(int x, int y, int w, int h);
	bool doDestroy();
};

#endif


