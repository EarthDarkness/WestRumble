#ifndef RENDERER_H
#define RENDERER_H

#include "resources.h"
#include "camera.h"
#include "..\game\TileMap.h"
#include "..\game\Actors.h"
#include "font.h"


class renderer
{
private:
	resources* _res;
public:
	float _scale;

	renderer();
	~renderer();

	void init(resources* data);

	void renderMap(TileMap& map, camera& cam, bool top);
	void renderActor(Actors& entry, int x, int y, int w = -1, int h = -1);//unused
	void renderActorInMap(Actors& entry, camera& cam, int w = -1, int h = -1);//unised
	void renderActorInMapCenter(Actors& entry, camera& cam, int offX = 0, int offY = 0);
	void renderSprite(int index, int x, int y, int w = -1, int h = -1);
	void renderText(const char* text, font& f, int x, int y);
	void renderTextInMapCenter(const char* text, camera& cam, font& f, int x, int y, int offX = 0, int offY = 0);

	void playSound(const char* name);
	void playMusic(const char* name, bool loop, bool force);
	void stopMusic();

};


#endif