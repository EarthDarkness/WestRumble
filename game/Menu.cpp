#include "Menu.h"
#include "enum.h"
#include "Props.h"

Menu::Menu(){
	_state = MAINMENU;

	_logo.image_id = 0;

	_play.image_id = 0;
	_tutorial.image_id = 0;
	_credits.image_id = 0;

	_next.image_id = 0;
	_prev.image_id = 0;
	_back.image_id = 0;

	_info[0].image_id = 0;
	_info[1].image_id = 0;
	_info[2].image_id = 0;
	_info[3].image_id = 0;

	_page = 0;

}
Menu::~Menu(){
}

void Menu::init(){
	_state = MAINMENU;

	_logo.image_id = IMG_GFX::logo;

	_play.image_id = IMG_UI::play;
	_tutorial.image_id = IMG_UI::tutorial;
	_credits.image_id = IMG_UI::credits;

	_next.image_id = IMG_UI::move;
	_prev.image_id = IMG_UI::move;
	_back.image_id = IMG_UI::exit;

	_info[0].image_id = IMG_TUT::objectives;
	_info[1].image_id = IMG_TUT::controls;
	_info[2].image_id = IMG_TUT::rules;
	_info[3].image_id = IMG_TUT::actions;

	_page = 0;
}

void Menu::setGUI(int width, int height){
	int lw = 840;
	int lh = 420;
	scaleHeight(height/2,lw,lh);
	if(lw>width)
		scaleWidth(width,lw,lh);
	_logo.setPos(width/2-lw/2,0,lw,lh);

	int ss;//space size
	int is;//item size
	fitInSpace(height/2,3,0.9f,ss,is);

	lw = 450;
	lh = 180;
	scaleHeight(is,lw,lh);
	if(lw>width)
		scaleWidth(width,lw,lh);

	int xx = width/2-lw/2;

	_play.setPos(xx,height/2+ss/2+0*(lh+ss),lw,lh);
	_tutorial.setPos(xx,height/2+ss/2+1*(lh+ss),lw,lh);
	_credits.setPos(xx,height/2+ss/2+2*(lh+ss),lw,lh);
	
	bool portrait = (width<height);
	
	_next.setPos(width-100,0,100,100);
	_prev.setPos(0,0,100,100);
	_back.setPos(width-96,height-96,96,96);
	for(int i=0;i<4;++i){
		if(portrait){
			_info[i].setPos(0,height/2-width/2,width,width);
		}else{
			_info[i].setPos(width/2-height/2,0,height,height);
		}
	}


}

