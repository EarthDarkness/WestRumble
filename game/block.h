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

	block* getBlock();

	void encode(char* data, int& len);
	void decode(char* data);
};
#endif