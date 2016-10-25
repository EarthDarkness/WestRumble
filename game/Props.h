
#ifndef PROPS_H
#define PROPS_H

#include <vector>
#include <memory.h>
#include "..\engine\animation.h"

extern std::vector<int> G_Anim;

extern animation VFX_gunfire;
extern animation VFX_gunflare;
extern animation VFX_explosion;

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

extern animation BMB_S_0;
extern animation BMB_S_30;
extern animation BMB_S_80;
extern animation BMB_S_180;
extern animation BMB_S_250;
extern animation BMB_S_300;

extern animation BMB_T_0;
extern animation BMB_T_30;
extern animation BMB_T_80;
extern animation BMB_T_180;
extern animation BMB_T_250;
extern animation BMB_T_300;

extern animation BMB_H_0;
extern animation BMB_H_30;
extern animation BMB_H_80;
extern animation BMB_H_180;
extern animation BMB_H_250;
extern animation BMB_H_300;

extern animation BLK_box;
extern animation BLK_barrel;

extern animation UI_action;
extern animation UI_ovelay;

struct IMG_UI{
	static int endturn;
	static int move;
	static int play;
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


int pushAnimation(int* frames, int size);
void loadAnimation(const char* file, animation& anim, int stt, resources& res);
void loadProps(resources& res);

#endif