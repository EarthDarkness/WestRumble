#include "renderer.h"
#include "translator.h"

extern SDL_Renderer* engRenderer;

renderer::renderer(){
	_res = NULL;
	_scale = 1.0f;
}
renderer::~renderer(){
}

void renderer::init(resources* data){
	_res = data;
}

void renderer::renderMap(TileMap& map, camera& cam, bool top){
	SDL_Rect pos;
	SDL_Texture* buf = NULL;

	//int xx = 300;
	//int yy = 100;
		
	for(int j=0;j<map._map.height();++j){
		for(int i=0;i<map._map.width();++i){
			matToWin(i,j,cam.getX(),cam.getY(),cam.getScale(),pos.x,pos.y);
			//pos.x = (int)(1.41f*0.5f*(float)(i-j)*32.0f)+xx;
			//pos.y = (int)(1.41f*0.25f*(float)(i+j)*32.0f)+yy;
			if(top){
				if(strcmp(map._map.at(i,j).up.c_str(),"NULL")==0)
					continue;
				buf = _res->getImage(map._map.at(i,j).up.c_str());
			}else{
				if(strcmp(map._map.at(i,j).bot.c_str(),"NULL")==0)
					continue;
				buf = _res->getImage(map._map.at(i,j).bot.c_str());
			}
			SDL_QueryTexture(buf,NULL,NULL,&pos.w,&pos.h);
			SDL_RenderCopy(engRenderer,buf,NULL,&pos);
		}
	}
}
void renderer::renderActor(Actors& entry, int x, int y, int w, int h){
	SDL_Rect pos;
	SDL_Texture* buf = NULL;
		
	pos.x = x;
	pos.y = y;
	
	if(strcmp(entry.getSprite(),"NULL")==0)
		return;
	buf = _res->getImage(entry.getSprite());

	pos.w = w;
	pos.h = h;
	if(w == -1 || h == -1)
		SDL_QueryTexture(buf,NULL,NULL,&pos.w,&pos.h);
	SDL_RenderCopy(engRenderer,buf,NULL,&pos);
}
void renderer::renderActorInMap(Actors& entry, camera& cam, int w, int h){
	SDL_Rect pos;
	SDL_Texture* buf = NULL;

	matToWin(entry.getX(),entry.getY(),cam.getX(),cam.getY(),cam.getScale(),pos.x,pos.y);

	if(strcmp(entry.getSprite(),"NULL")!=0)
		buf = _res->getImage(entry.getSprite());
	pos.w = w;
	pos.h = h;
	if(w == -1 || h == -1)
		SDL_QueryTexture(buf,NULL,NULL,&pos.w,&pos.h);
	//pos.x -= cam.getScale()/2;
	//pos.y -= cam.getScale()/4;
	SDL_RenderCopy(engRenderer,buf,NULL,&pos);
}
void renderer::renderActorInMapCenter(Actors& entry, camera& cam, int offX, int offY){
	SDL_Rect pos;
	SDL_Texture* buf = NULL;

	if(strcmp(entry.getSprite(),"NULL")==0)
		return;

	matToWin(entry.getX(),entry.getY(),cam.getX(),cam.getY(),cam.getScale(),pos.x,pos.y);

	buf = _res->getImage(entry.getSprite());
	SDL_QueryTexture(buf,NULL,NULL,&pos.w,&pos.h);

	pos.x += (cam.getScale()-pos.w)/2 + offX;
	pos.y += (cam.getScale()/2-pos.h)/2 + offY;


	SDL_RenderCopy(engRenderer,buf,NULL,&pos);
}
void renderer::renderSprite(const char* name, int x, int y, int w, int h){
	SDL_Texture* buf = _res->getImage(name);
	if(buf == NULL)
		return;
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = h;
	if(w == -1 || h == -1)
		SDL_QueryTexture(buf,NULL,NULL,&pos.w,&pos.h);
	SDL_RenderCopy(engRenderer,buf,NULL,&pos);
}
void renderer::renderText(const char* text, font& f, int x, int y){
	int len = strlen(text);
	int w = f.width();
	int h = f.height();
	int line = 0;
	for(int i=0;i<len;++i){
		const char* buf = f.getChar(text[i]);
		if(buf == NULL)
			continue;
		renderSprite(buf,x+i*w,y+line*h,w,h);
	}
}
void renderer::renderTextInMapCenter(const char* text, camera& cam, font& f, int x, int y, int offX, int offY){
	int len = strlen(text);
	int w = f.width();
	int h = f.height();
	int line = 0;
	int xx;
	int yy;
	matToWin(x,y,cam.getX(),cam.getY(),cam.getScale(),xx,yy);
	xx += (cam.getScale()-w)/2 + offX;
	yy += (cam.getScale()/2-h)/2 + offY;

	for(int i=0;i<len;++i){
		const char* buf = f.getChar(text[i]);
		if(buf == NULL)
			continue;
		renderSprite(buf,xx+i*w,yy+line*h,w,h);
	}
}

void renderer::playSound(const char* name){
	Mix_PlayChannel(-1,_res->getSound(name),0);
}
void renderer::playMusic(const char* name, bool loop, bool force = true){
	if(Mix_PlayingMusic() != 0)
		if(force)
			Mix_HaltMusic();
		else
			return;
	if(loop)
		Mix_PlayMusic(_res->getMusic(name),-1);
	else
		Mix_PlayMusic(_res->getMusic(name),1);
}
void renderer::stopMusic(){
	Mix_HaltMusic();
}