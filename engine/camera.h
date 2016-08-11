#ifndef CAMERA_H
#define CAMERA_H


class camera
{
private:
	int _x;
	int _y;

	int _scale;
public:
	camera();
	~camera();

	void setPos(int x, int y);
	void addPos(int x, int y);
	void setScale(int scale);
	void addScale(int scale);

	int getX();
	int getY();
	int getScale();

};

class camCtrl
{
private:
	int _x0,_x;
	int _y0,_y;
	float _time;
	float _speed;

	camera* _cam;

public:
	camCtrl();
	~camCtrl();

	void setCam(camera* cam);
	void setSpeed(float speed);

	void goTo(int x, int y);
	void goFromTo(int x0, int y0, int x, int y);

	void update();

	bool isDone();

};

#endif