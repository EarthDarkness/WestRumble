#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <string>
#include <sstream>

#include <SDL.h>

#define ROOT2 1.41421356

inline int powi(int base, int exp){
	int buf = 1;
	if(exp == 0)
		return buf;
	for(int i=0;i<exp;++i)
		buf*=base;
	return buf;
}
//old unused
inline void winToMat(int winX, int winY, int camX, int camY, float scale, int& matX, int& matY){
	float posX = (float)(winX-camX);
	float posY = (float)(winY-camY);
	matX = (int)((
		+ROOT2*posX*0.5f
		+ROOT2*posY
		)/scale-0.25f
		);
	matY = (int)((
		-ROOT2*posX*0.5f
		+ROOT2*posY
		)/scale+0.5f
		);
}
//old unused
inline void matToWin(int matX, int matY, int camX, int camY, float scale, int& winX, int& winY){
	float conX = (float)(matX-matY);
	float conY = (float)(matX+matY);
	winX = (int)(ROOT2*conX*scale*0.5f)+camX;
	winY = (int)(ROOT2*conY*scale*0.25f)+camY;
}

inline void winToMat(int winX, int winY, int camX, int camY, int width, int& matX, int& matY){
	float posX = (float)(winX-camX+width/2);
	float posY = (float)(winY-camY);
	float dist = (float)(width)/ROOT2;
	matX = (int)((
		+ROOT2*posX*0.5f
		+ROOT2*posY
		)/dist-0.5f+1.0f
		)-1;
	matY = (int)((
		-ROOT2*posX*0.5f
		+ROOT2*posY
		)/dist+0.5f+1.0f
		)-1;
}
inline void matToWin(int matX, int matY, int camX, int camY, int width, int& winX, int& winY){
	float conX = (float)(matX-matY);
	float conY = (float)(matX+matY);
	float dist = (float)(width)/(2.0f*ROOT2);
	winX = (int)(ROOT2*conX*dist)+camX-width/2;
	winY = (int)(ROOT2*conY*dist*0.5f)+camY;
}

inline string i_to_str(int value){
	string buf;
	bool start = false;
	int min = 0;
	for(int i=9;i>=0;--i){
		int p = powi(10,i);
		int div = (value-min)/p;
		min += p*div;
		if(start){
			buf += (char)(48+div);
			continue;
		}
		if(div > 0){
			start = true;
			buf += (char)(48+div);
		}
	}
	return buf;
}

inline void readRW(std::stringstream& iss, const char* file){
	SDL_RWops* rw;
	char* buf;
	int size;

	rw = SDL_RWFromFile(file,"rb");
	size = SDL_RWsize(rw);
	buf = new char[size];

	SDL_RWread(rw,buf,1,size);
	//size = strlen((char*)rw->hidden.mem.stop);
	string str;
	str.assign(buf,size);
	iss << str;
	//size = iss.str().size();

	delete[] buf;

	SDL_RWclose(rw);

	return;

}

#endif