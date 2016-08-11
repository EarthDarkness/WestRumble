#ifndef MOTION_H
#define MOTION_H

#include "../game/TileMap.h"
#include "camera.h"
#include <vector>

#define WALKSPEED 20

using namespace std;

enum dir_enum{
	DIR_N = 0,
	DIR_S,
	DIR_E,
	DIR_W
};
void dirToCoord(int dir,int* vec);

class a_node
{
public:
	a_node* _parent;
	int _pos;
	int _x,_y;
	float _f;	//total cost
	float _g;	//movement cost
	float _h;	//guess, using linear distance

	a_node(){}
	~a_node(){}
};


void killVec(vector<a_node*>& vec);

int findNode(vector<a_node*>& vec, a_node* data);
int findNode(vector<a_node*>& vec, int x, int y);
int minNode(vector<a_node*>& vec);
float distance(float x1, float y1, float x2, float y2);

void getPatch(matMN<Tile>& map, int x0, int y0, int x, int y, vector<int>& out);

class motion
{
private:
	static camera* _cam;
	//TODO actor pointer to draw fake player walking while true is not visible

	vector<int> _path;
	int _xm;//matrix initial x
	int _ym;//matrix initial y

	int _x0,_y0;//currernt move initial win pos
	int _xv,_yv;//currernt move vector win dir
	int _count;//time from 0 to WALKSPEED

	bool _moveDone;
	animation* _anim;
	bool _animDone;
public:
	motion();
	~motion();

	static void init(camera* cam);

	void waitForMotion(matMN<Tile>& map, int x0, int y0, int x, int y, animation* anim);
	void vaitForAnimation(animation* anim);

	void update();
	bool isDone();

};








#endif