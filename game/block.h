#ifndef BLOCK_H
#define BLOCK_H

#include "Actors.h"

class block : public Actors
{
private:
	int _data;
public:
	block();
	~block();

	void setData(int data);
	int getData();


};
#endif