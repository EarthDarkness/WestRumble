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
	for(int i=0;i<image.size();++i)
		SDL_DestroyTexture(image[i]);
	for(map<string,Mix_Chunk*>::iterator it = sound.begin();it != sound.end();++it)
		Mix_FreeChunk(it->second);
	for(map<string,Mix_Music*>::iterator it = music.begin();it != music.end();++it)
		Mix_FreeMusic(it->second);
	
}

void resources::eraseImage(int index){
	SDL_DestroyTexture(image[index]);
	image[index] = NULL;
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

int resources::loadImage(const char* path){
	SDL_Texture* buf = NULL;
	buf = IMG_LoadTexture(engRenderer, path);
	if(buf == NULL){
		cerr << "Couldn't load image " << path << endl;
		return 0;
	}else{
		image.push_back(buf);
		return image.size()-1;
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

SDL_Texture* resources::getImage(int index){
	if(index < 0 || index >= image.size())
		return NULL;
	return image[index];
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