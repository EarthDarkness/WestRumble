#include "resources.h"
extern SDL_Renderer *engRenderer;

#include <iostream>
using namespace std;

resources::resources(){
}
resources::~resources(){
	clear();
}

void resources::clear(){
	for(map<string,SDL_Texture*>::iterator it = image.begin();it != image.end();++it)
		SDL_DestroyTexture(it->second);
	for(map<string,Mix_Chunk*>::iterator it = sound.begin();it != sound.end();++it)
		Mix_FreeChunk(it->second);
	for(map<string,Mix_Music*>::iterator it = music.begin();it != music.end();++it)
		Mix_FreeMusic(it->second);
	
}

void resources::eraseImage(const char* name){
	map<string, SDL_Texture*>::iterator it = image.find(name);
	if(it != image.end())
		SDL_DestroyTexture(it->second);
}
void resources::eraseSound(const char* name){
	map<string,Mix_Chunk*>::iterator it = sound.find(name);
	if(it != sound.end())
		Mix_FreeChunk(it->second);
}
void resources::eraseMusic(const char* name){
	map<string,Mix_Music*>::iterator it = music.find(name);
	if(it != music.end())
		Mix_FreeMusic(it->second);
}

int resources::loadImage(const char* path, const char* name){
	SDL_Texture* buf = NULL;
	buf = IMG_LoadTexture(engRenderer, path);
	if(buf == NULL){
		cerr << "Couldn't load image " << path << endl;
		return -1;
	}else{
		image[name] = buf;
		return 0;
	}
}
int resources::loadSound(const char* path, const char* name){
	Mix_Chunk* buf = NULL;
	buf = Mix_LoadWAV(path);
	if(buf == NULL){
		cerr << "Couldn't load sound " << path << endl;
		return -1;
	}else{
		sound[name] = buf;
		return 0;
	}
}
int resources::loadMusic(const char* path, const char* name){
	Mix_Music* buf = NULL;
	buf = Mix_LoadMUS(path);
	if(buf == NULL){
		cerr << "Couldn't load music " << path << endl;
		return -1;
	}else{
		music[name] = buf;
		return 0;
	}
}

SDL_Texture* resources::getImage(const char* name){
	if(image.find(name) == image.end())
		return NULL;
	return image[name];
}
Mix_Chunk* resources::getSound(const char* name){
	if(sound.find(name) == sound.end())
		return NULL;
	return sound[name];
}
Mix_Music* resources::getMusic(const char* name){
	if(music.find(name) == music.end())
		return NULL;
	return music[name];
}