#include "enum.h"


int rseed=0;

int abs(int a){
	if(a<0)
		return -a;
	return a;
}

int randn(){
	rseed = abs((137737*rseed + 746773)%362431);
	return rseed;
}

void fitInSpace(int total, int num, float cover, int& space, int& size){
	size = (int)((float)total*cover)/num;
	space = (int)((float)total*(1.0f-cover))/num+1;
}
void scaleWidth(int size, int& width, int& height){
	height = (int)((float)height*((float)size/(float)width));
	width = size;
}
void scaleHeight(int size, int& width, int& height){
	width = (int)((float)width*((float)size/(float)height));
	height = size;
}