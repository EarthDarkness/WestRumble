#include "Props.h"
#include "..\engine\resources.h"
#include "..\engine\translator.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

std::vector<int> G_Anim;

animation VFX_gunfire;
animation VFX_gunflare;
animation VFX_explosion;

animation* VFX_TABLE[] = {
	NULL,
	&VFX_gunfire,
	&VFX_gunflare,
	&VFX_explosion
};

animation CHR_char_P1;
animation CHR_char_P2;

animation PWP_barrel;
animation PWP_boot;
animation PWP_detonator;
animation PWP_dynamite;
animation PWP_fuse;
animation PWP_glove;
animation PWP_gunpowder;
animation PWP_rope;
animation PWP_star;

animation* PWP_TABLE[] = {
	NULL,
	&PWP_dynamite,
	&PWP_gunpowder,
	&PWP_boot,
	&PWP_star,
	NULL,
	&PWP_fuse,
	&PWP_glove,
	&PWP_barrel,
	&PWP_rope,
	&PWP_detonator
};


animation BMB_SINGLE;
animation BMB_TRIPLE;
animation BMB_SEPTUPLE;

animation* BMB_TABLE[]={
	NULL,
	&BMB_SINGLE,
	&BMB_TRIPLE,
	&BMB_SEPTUPLE
};


animation BLK_box;
animation BLK_barrel;

animation* BLK_TABLE[]={
	NULL,
	&BLK_box,
	&BLK_barrel
};

animation UI_numpad;
animation UI_action;
animation UI_ovelay;

int IMG_INVALID = 0;
int IMG_TRANSPARENT = 1;

int IMG_UI::endturn = 0;
int IMG_UI::move = 0;
int IMG_UI::play = 0;
int IMG_UI::server = 0;
int IMG_UI::client = 0;
int IMG_UI::tutorial = 0;
int IMG_UI::credits = 0;
int IMG_UI::exit = 0;
int IMG_UI::num[10] = {0,0,0,0,0,0,0,0,0,0};
int IMG_UI::stgthumb0 = 0;

int IMG_TUT::objectives = 0;
int IMG_TUT::controls = 0;
int IMG_TUT::rules = 0;
int IMG_TUT::actions = 0;

int IMG_GFX::logo = 0;
int IMG_GFX::wood = 0;

int IMG_GFX::select = 0;

int IMG_GFX::char_black = 0;
int IMG_GFX::char_white = 0;

int IMG_GFX::boot = 0;
int IMG_GFX::bomb = 0;
int IMG_GFX::gunpowder = 0;
int IMG_GFX::timer = 0;

int IMG_GFX::win1 = 0;
int IMG_GFX::win2 = 0;
int IMG_GFX::tie = 0;


int IMG_OL::bomb = 0;
int IMG_OL::barrel = 0;
int IMG_OL::downtimer = 0;
int IMG_OL::uptimer = 0;
int IMG_OL::gunfire = 0;
int IMG_OL::rope = 0;
int IMG_OL::launch = 0;
int IMG_OL::walk = 0;
int IMG_OL::marker = 0;


int IMG_ACT::bomb = 0;
int IMG_ACT::barrel = 0;
int IMG_ACT::detonate = 0;
int IMG_ACT::grab = 0;
int IMG_ACT::move = 0;
int IMG_ACT::shot = 0;
int IMG_ACT::stun = 0;
int IMG_ACT::launch = 0;
int IMG_ACT::timeup = 0;
int IMG_ACT::timedown = 0;


int IMG_BLK::box = 0;
int IMG_BLK::barrel = 0;


int pushAnimation(int* frames, int size){
	for(int i=0;i<size;++i)
		G_Anim.push_back(frames[i]);
	return G_Anim.size()-size;
}
void loadAnimation(const char* file, animation& anim, int stt, resources& res){
	stringstream in;
	vector<string> data;
	vector<int> index;

	readRW(in,file);

	string str;
	int size;
	float sp;
	char loop;
	bool l;

	in >> size;
	data.reserve(size);
	index.reserve(size);
	for(int i=0;i<size;++i){
		str.clear();
		in >> str;
		data.push_back(str);
	}
	const char* path = "resources/image/";
	for(int i=0;i<data.size();++i){
		string imgpath = path;
		imgpath += data[i];

		index.push_back(res.loadImage(imgpath.c_str()));
	}
	str.clear();
	in >> str;
	in >> size;
	in >> sp;
	in >> loop;

	l = (loop == 'L');

	int val = pushAnimation(&(index[0]),size); 

	anim.loadAnim(stt,val,size,sp,l);
}
void loadAnimation(int sprite, animation& anim, int stt, resources& res){
	int val = pushAnimation(&sprite,1); 

	anim.loadAnim(stt,val,1,1.0f,true);
}

void intLoadImages(resources& res){
	res.loadImage("resources/image/invalid.png");
	res.loadImage("resources/image/transparent.png");
	{
		int zero=0;
		pushAnimation(&zero,1);
	}

	IMG_GFX::logo = res.loadImage("resources/image/GFX_logo.png");
	IMG_GFX::wood = res.loadImage("resources/image/BGN_title.png");

	IMG_UI::play = res.loadImage("resources/image/UI_play.png");
	IMG_UI::server = res.loadImage("resources/image/UI_server.png");
	IMG_UI::client = res.loadImage("resources/image/UI_client.png");
	IMG_UI::tutorial = res.loadImage("resources/image/UI_tutorial.png");
	IMG_UI::credits = res.loadImage("resources/image/UI_credits.png");
	IMG_UI::num[0] = res.loadImage("resources/image/UI_num_0.png");
	IMG_UI::num[1] = res.loadImage("resources/image/UI_num_1.png");
	IMG_UI::num[2] = res.loadImage("resources/image/UI_num_2.png");
	IMG_UI::num[3] = res.loadImage("resources/image/UI_num_3.png");
	IMG_UI::num[4] = res.loadImage("resources/image/UI_num_4.png");
	IMG_UI::num[5] = res.loadImage("resources/image/UI_num_5.png");
	IMG_UI::num[6] = res.loadImage("resources/image/UI_num_6.png");
	IMG_UI::num[7] = res.loadImage("resources/image/UI_num_7.png");
	IMG_UI::num[8] = res.loadImage("resources/image/UI_num_8.png");
	IMG_UI::num[9] = res.loadImage("resources/image/UI_num_9.png");
	IMG_UI::stgthumb0 = res.loadImage("resources/image/UI_STG_thumb_0.png");


	IMG_TUT::actions = res.loadImage("resources/image/TUT_actions.png");
	IMG_TUT::controls = res.loadImage("resources/image/TUT_controls.png");
	IMG_TUT::objectives = res.loadImage("resources/image/TUT_objectives.png");
	IMG_TUT::rules = res.loadImage("resources/image/TUT_rules.png");

	IMG_UI::move = res.loadImage("resources/image/UI_move.png");
	IMG_UI::endturn = res.loadImage("resources/image/UI_endturn.png");

	IMG_OL::bomb = res.loadImage("resources/image/OL_bomb.png");
	IMG_OL::barrel = res.loadImage("resources/image/OL_barrel.png");
	IMG_OL::downtimer = res.loadImage("resources/image/OL_downtimer.png");
	IMG_OL::uptimer = res.loadImage("resources/image/OL_uptimer.png");
	IMG_OL::gunfire = res.loadImage("resources/image/OL_gunfire.png");
	IMG_OL::rope = res.loadImage("resources/image/OL_rope.png");
	IMG_OL::launch = res.loadImage("resources/image/OL_throw.png");
	IMG_OL::walk = res.loadImage("resources/image/OL_walk.png");
	IMG_OL::marker = res.loadImage("resources/image/OL_marker.png");

	IMG_ACT::barrel = res.loadImage("resources/image/GFX_ACT_barrel.png");
	IMG_ACT::bomb = res.loadImage("resources/image/GFX_ACT_bomb.png");
	IMG_ACT::detonate = res.loadImage("resources/image/GFX_ACT_detonate.png");
	IMG_ACT::grab = res.loadImage("resources/image/GFX_ACT_grab.png");
	IMG_ACT::move = res.loadImage("resources/image/GFX_ACT_move.png");
	IMG_ACT::shot = res.loadImage("resources/image/GFX_ACT_shot.png");
	IMG_ACT::stun = res.loadImage("resources/image/GFX_ACT_stun.png");
	IMG_ACT::launch = res.loadImage("resources/image/GFX_ACT_throw.png");
	IMG_ACT::timedown = res.loadImage("resources/image/GFX_ACT_timerdown.png");
	IMG_ACT::timeup = res.loadImage("resources/image/GFX_ACT_timerup.png");

	IMG_BLK::box = res.loadImage("resources/image/BLK_box.png");
	IMG_BLK::barrel = res.loadImage("resources/image/BLK_barrel.png");

	IMG_GFX::win1 = res.loadImage("resources/image/jogador1.png");
	IMG_GFX::win2 = res.loadImage("resources/image/jogador2.png");
	IMG_GFX::tie = res.loadImage("resources/image/draw.png");

	IMG_GFX::bomb = res.loadImage("resources/image/GFX_UI_bomb.png");
	IMG_GFX::gunpowder = res.loadImage("resources/image/GFX_UI_gunpowder.png");
	IMG_GFX::boot = res.loadImage("resources/image/GFX_UI_boot.png");
	IMG_GFX::timer = res.loadImage("resources/image/GFX_UI_timer.png");

	IMG_GFX::char_black = res.loadImage("resources/image/UI_char_black.png");
	IMG_GFX::char_white = res.loadImage("resources/image/UI_char_white.png");
	IMG_GFX::select = res.loadImage("resources/image/select.png");

	IMG_UI::exit = res.loadImage("resources/image/exit.png");

}
void intLoadAnimations(resources& res){

	loadAnimation("resources/VFX_exp.txt",VFX_explosion,ANIM_IDLE,res);

	loadAnimation("resources/VFX_gunfire_left.txt",VFX_gunfire,ANIM_IDLE_L,res);
	loadAnimation("resources/VFX_gunfire_right.txt",VFX_gunfire,ANIM_IDLE_R,res);
	loadAnimation("resources/VFX_gunfire_up.txt",VFX_gunfire,ANIM_IDLE_U,res);
	loadAnimation("resources/VFX_gunfire_down.txt",VFX_gunfire,ANIM_IDLE_D,res);

	loadAnimation("resources/VFX_gunflare_left.txt",VFX_gunflare,ANIM_IDLE_L,res);
	loadAnimation("resources/VFX_gunflare_right.txt",VFX_gunflare,ANIM_IDLE_R,res);
	loadAnimation("resources/VFX_gunflare_up.txt",VFX_gunflare,ANIM_IDLE_U,res);
	loadAnimation("resources/VFX_gunflare_down.txt",VFX_gunflare,ANIM_IDLE_D,res);



	loadAnimation("resources/PWP_barrel.txt",PWP_barrel,ANIM_IDLE,res);
	loadAnimation("resources/PWP_boot.txt",PWP_boot,ANIM_IDLE,res);
	loadAnimation("resources/PWP_detonator.txt",PWP_detonator,ANIM_IDLE,res);
	loadAnimation("resources/PWP_dynamite.txt",PWP_dynamite,ANIM_IDLE,res);
	loadAnimation("resources/PWP_fuse.txt",PWP_fuse,ANIM_IDLE,res);
	loadAnimation("resources/PWP_glove.txt",PWP_glove,ANIM_IDLE,res);
	loadAnimation("resources/PWP_gunpowder.txt",PWP_gunpowder,ANIM_IDLE,res);
	loadAnimation("resources/PWP_rope.txt",PWP_rope,ANIM_IDLE,res);
	loadAnimation("resources/PWP_star.txt",PWP_star,ANIM_IDLE,res);


	loadAnimation("resources/BMB_S_0.txt",BMB_SINGLE,ANIM_0,res);
	loadAnimation("resources/BMB_S_30.txt",BMB_SINGLE,ANIM_30,res);
	loadAnimation("resources/BMB_S_80.txt",BMB_SINGLE,ANIM_80,res);
	loadAnimation("resources/BMB_S_180.txt",BMB_SINGLE,ANIM_180,res);
	loadAnimation("resources/BMB_S_250.txt",BMB_SINGLE,ANIM_250,res);
	loadAnimation("resources/BMB_S_300.txt",BMB_SINGLE,ANIM_300,res);

	loadAnimation("resources/BMB_T_0.txt",BMB_TRIPLE,ANIM_0,res);
	loadAnimation("resources/BMB_T_30.txt",BMB_TRIPLE,ANIM_30,res);
	loadAnimation("resources/BMB_T_80.txt",BMB_TRIPLE,ANIM_80,res);
	loadAnimation("resources/BMB_T_180.txt",BMB_TRIPLE,ANIM_180,res);
	loadAnimation("resources/BMB_T_250.txt",BMB_TRIPLE,ANIM_250,res);
	loadAnimation("resources/BMB_T_300.txt",BMB_TRIPLE,ANIM_300,res);

	loadAnimation("resources/BMB_H_0.txt",BMB_SEPTUPLE,ANIM_0,res);
	loadAnimation("resources/BMB_H_30.txt",BMB_SEPTUPLE,ANIM_30,res);
	loadAnimation("resources/BMB_H_80.txt",BMB_SEPTUPLE,ANIM_80,res);
	loadAnimation("resources/BMB_H_180.txt",BMB_SEPTUPLE,ANIM_180,res);
	loadAnimation("resources/BMB_H_250.txt",BMB_SEPTUPLE,ANIM_250,res);
	loadAnimation("resources/BMB_H_300.txt",BMB_SEPTUPLE,ANIM_300,res);



	loadAnimation("resources/CHR_black_idle_left.txt",CHR_char_P1,ANIM_IDLE_L,res);
	loadAnimation("resources/CHR_black_idle_right.txt",CHR_char_P1,ANIM_IDLE_R,res);
	loadAnimation("resources/CHR_black_idle_up.txt",CHR_char_P1,ANIM_IDLE_U,res);
	loadAnimation("resources/CHR_black_idle_down.txt",CHR_char_P1,ANIM_IDLE_D,res);
	loadAnimation("resources/CHR_black_ready_left.txt",CHR_char_P1,ANIM_READY_L,res);
	loadAnimation("resources/CHR_black_ready_right.txt",CHR_char_P1,ANIM_READY_R,res);
	loadAnimation("resources/CHR_black_ready_up.txt",CHR_char_P1,ANIM_READY_U,res);
	loadAnimation("resources/CHR_black_ready_down.txt",CHR_char_P1,ANIM_READY_D,res);
	loadAnimation("resources/CHR_black_shot_left.txt",CHR_char_P1,ANIM_SHOT_L,res);
	loadAnimation("resources/CHR_black_shot_right.txt",CHR_char_P1,ANIM_SHOT_R,res);
	loadAnimation("resources/CHR_black_shot_up.txt",CHR_char_P1,ANIM_SHOT_U,res);
	loadAnimation("resources/CHR_black_shot_down.txt",CHR_char_P1,ANIM_SHOT_D,res);
	loadAnimation("resources/CHR_black_walk_left.txt",CHR_char_P1,ANIM_WALK_L,res);
	loadAnimation("resources/CHR_black_walk_right.txt",CHR_char_P1,ANIM_WALK_R,res);
	loadAnimation("resources/CHR_black_walk_up.txt",CHR_char_P1,ANIM_WALK_U,res);
	loadAnimation("resources/CHR_black_walk_down.txt",CHR_char_P1,ANIM_WALK_D,res);

	loadAnimation("resources/CHR_white_idle_left.txt",CHR_char_P2,ANIM_IDLE_L,res);
	loadAnimation("resources/CHR_white_idle_right.txt",CHR_char_P2,ANIM_IDLE_R,res);
	loadAnimation("resources/CHR_white_idle_up.txt",CHR_char_P2,ANIM_IDLE_U,res);
	loadAnimation("resources/CHR_white_idle_down.txt",CHR_char_P2,ANIM_IDLE_D,res);
	loadAnimation("resources/CHR_white_ready_left.txt",CHR_char_P2,ANIM_READY_L,res);
	loadAnimation("resources/CHR_white_ready_right.txt",CHR_char_P2,ANIM_READY_R,res);
	loadAnimation("resources/CHR_white_ready_up.txt",CHR_char_P2,ANIM_READY_U,res);
	loadAnimation("resources/CHR_white_ready_down.txt",CHR_char_P2,ANIM_READY_D,res);
	loadAnimation("resources/CHR_white_shot_left.txt",CHR_char_P2,ANIM_SHOT_L,res);
	loadAnimation("resources/CHR_white_shot_right.txt",CHR_char_P2,ANIM_SHOT_R,res);
	loadAnimation("resources/CHR_white_shot_up.txt",CHR_char_P2,ANIM_SHOT_U,res);
	loadAnimation("resources/CHR_white_shot_down.txt",CHR_char_P2,ANIM_SHOT_D,res);
	loadAnimation("resources/CHR_white_walk_left.txt",CHR_char_P2,ANIM_WALK_L,res);
	loadAnimation("resources/CHR_white_walk_right.txt",CHR_char_P2,ANIM_WALK_R,res);
	loadAnimation("resources/CHR_white_walk_up.txt",CHR_char_P2,ANIM_WALK_U,res);
	loadAnimation("resources/CHR_white_walk_down.txt",CHR_char_P2,ANIM_WALK_D,res);

	loadAnimation(IMG_BLK::box,BLK_box,ANIM_IDLE,res);
	loadAnimation(IMG_BLK::barrel,BLK_barrel,ANIM_IDLE,res);

	for(int i=0;i<10;++i)
		loadAnimation(IMG_UI::num[i],UI_numpad,i,res);

	loadAnimation(IMG_TRANSPARENT,UI_action,ANIM_NONE,res);
	loadAnimation(IMG_ACT::move,UI_action,ANIM_MOVE,res);
	loadAnimation(IMG_ACT::bomb,UI_action,ANIM_BOMB,res);
	loadAnimation(IMG_ACT::shot,UI_action,ANIM_SHOT,res);
	loadAnimation(IMG_ACT::timeup,UI_action,ANIM_TIMERUP,res);
	loadAnimation(IMG_ACT::timedown,UI_action,ANIM_TIMERDOWN,res);
	loadAnimation(IMG_ACT::grab,UI_action,ANIM_GRAB,res);
	loadAnimation(IMG_ACT::launch,UI_action,ANIM_LAUNCH,res);
	loadAnimation(IMG_ACT::barrel,UI_action,ANIM_BARREL,res);
	loadAnimation(IMG_ACT::stun,UI_action,ANIM_STUN,res);
	loadAnimation(IMG_ACT::detonate,UI_action,ANIM_DETOANTE,res);

	loadAnimation(IMG_OL::marker,UI_ovelay,ANIM_NONE,res);
	loadAnimation(IMG_OL::walk,UI_ovelay,ANIM_MOVE,res);
	loadAnimation(IMG_OL::bomb,UI_ovelay,ANIM_BOMB,res);
	loadAnimation(IMG_OL::gunfire,UI_ovelay,ANIM_SHOT,res);
	loadAnimation(IMG_OL::uptimer,UI_ovelay,ANIM_TIMERUP,res);
	loadAnimation(IMG_OL::downtimer,UI_ovelay,ANIM_TIMERDOWN,res);
	loadAnimation(IMG_OL::launch,UI_ovelay,ANIM_LAUNCH,res);
	loadAnimation(IMG_OL::barrel,UI_ovelay,ANIM_BARREL,res);
	loadAnimation(IMG_OL::rope,UI_ovelay,ANIM_STUN,res);


}

void loadProps(resources& res){

	intLoadImages(res);
	intLoadAnimations(res);


}











