
#ifndef PROPS_H
#define PROPS_H

#include <vector>
#include <memory.h>
#include "..\engine\animation.h"

extern std::vector<int> G_Anim;

extern animation VFX_gunfire;
extern animation VFX_gunflare;
extern animation VFX_explosion;

extern animation* VFX_TABLE[];

enum VFX_ID{
	VFX_ID_NONE=0,
	VFX_ID_GUNFLARE,
	VFX_ID_GUNFIRE,
	VFX_ID_EXPLOSION
};

extern animation CHR_char_P1;
extern animation CHR_char_P2;

extern animation PWP_barrel;
extern animation PWP_boot;
extern animation PWP_detonator;
extern animation PWP_dynamite;
extern animation PWP_fuse;
extern animation PWP_glove;
extern animation PWP_gunpowder;
extern animation PWP_rope;
extern animation PWP_star;

extern animation* PWP_TABLE[];

enum PWP_ID{
	PWP_ID_NONE=0,
	PWP_ID_BARREL,
	PWP_ID_BOOT,
	PWP_ID_DETOANTOR,
	PWP_ID_DYNAMITE,
	PWP_ID_FUSE,
	PWP_ID_GLOVE,
	PWP_ID_GUNPOWDER,
	PWP_ID_ROPE,
	PWP_ID_STAR
};

extern animation BMB_SINGLE;
extern animation BMB_TRIPLE;
extern animation BMB_SEPTUPLE;

extern animation* BMB_TABLE[];

enum BMB_ID{
	BMB_ID_NONE=0,
	BMB_ID_SINGLE,
	BMB_ID_TRIPLE,
	BMB_ID_SEPTUPLE
};

extern animation BMB_H_300;

extern animation BLK_box;
extern animation BLK_barrel;

extern animation* BLK_TABLE[];

enum BLK_ID{
	BLK_ID_NONE=0,
	BLK_ID_BOX,
	BLK_ID_BARREL
};

extern animation UI_action;
extern animation UI_ovelay;


extern int IMG_INVALID;
extern int IMG_TRANSPARENT;

struct IMG_UI{
	static int endturn;
	static int move;
	static int play;
	static int server;
	static int client;
	static int tutorial;
	static int credits;
	static int exit;

	IMG_UI(){
		memset(this,0,sizeof(IMG_UI));
	}
};
struct IMG_TUT{
	static int objectives;
	static int controls;
	static int rules;
	static int actions;

	IMG_TUT(){
		memset(this,0,sizeof(IMG_TUT));
	}
};
struct IMG_GFX{
	static int logo;
	static int wood;

	static int select;

	static int char_black;
	static int char_white;

	static int boot;
	static int bomb;
	static int gunpowder;
	static int timer;

	static int win1;
	static int win2;
	static int tie;


	IMG_GFX(){
		memset(this,0,sizeof(IMG_GFX));
	}
};
struct IMG_OL{
	static int bomb;
	static int barrel;
	static int downtimer;
	static int uptimer;
	static int gunfire;
	static int rope;
	static int launch;
	static int walk;
	static int marker;

	IMG_OL(){
		memset(this,0,sizeof(IMG_OL));
	}
};
struct IMG_ACT{
	static int bomb;
	static int barrel;
	static int detonate;
	static int grab;
	static int move;
	static int shot;
	static int stun;
	static int launch;
	static int timeup;
	static int timedown;

	IMG_ACT(){
		memset(this,0,sizeof(IMG_ACT));
	}
};
struct IMG_BLK{
	static int box;
	static int barrel;

	IMG_BLK(){
		memset(this,0,sizeof(IMG_BLK));
	}
};

enum ANIM_STATE_CHR{
	ANIM_IDLE=0,

	ANIM_IDLE_L=0,
	ANIM_IDLE_R=1,
	ANIM_IDLE_U=2,
	ANIM_IDLE_D=3,

	ANIM_READY_L=4,
	ANIM_READY_R=5,
	ANIM_READY_U=6,
	ANIM_READY_D=7,

	ANIM_SHOT_L=8,
	ANIM_SHOT_R=9,
	ANIM_SHOT_U=10,
	ANIM_SHOT_D=11,
};
enum ANIM_STATE_UI{
	ANIM_NONE=0,
	ANIM_MOVE=1,
	ANIM_BOMB=2,
	ANIM_SHOT=3,
	ANIM_TIMERUP=4,
	ANIM_TIMERDOWN=5,
	ANIM_GRAB=6,
	ANIM_LAUNCH=7,
	ANIM_BARREL=8,
	ANIM_STUN=9,
	ANIM_DETOANTE=10
};
enum ANIM_STATE_BMB{
	ANIM_0=0,
	ANIM_30,
	ANIM_80,
	ANIM_180,
	ANIM_250,
	ANIM_300
};


int pushAnimation(int* frames, int size);
void loadAnimation(const char* file, animation& anim, int stt, resources& res);
void loadProps(resources& res);

#endif