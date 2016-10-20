#ifndef RESOURCES_H
#define RESOURCES_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_Mixer.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

class resources
{
private:
	//map<string, SDL_Texture*> image;
	vector<SDL_Texture*> image;
	map<string,Mix_Chunk*> sound;
	map<string,Mix_Music*> music;

public:
	resources();
	~resources();

	void clear();

	void eraseImage(int index);
	void eraseSound(const char* name);
	void eraseMusic(const char* name);

	int loadImage(const char* path);
	int loadSound(const char* path, const char* name);
	int loadMusic(const char* path, const char* name);

	SDL_Texture* getImage(int index);
	Mix_Chunk* getSound(const char* name);
	Mix_Music* getMusic(const char* name);


};

#endif