#include "Menu.h"
#include "enum.h"

Menu::Menu(){
	_state = MAINMENU;

	_logo.image_id = "GFX_logo";

	_play.image_id = "UI_play";
	_tutorial.image_id = "UI_tutorial";
	_credits.image_id = "UI_credits";

	_next.image_id = "UI_move";
	_prev.image_id = "UI_move";
	_back.image_id = "exit.png";

	_info[0].image_id = "TUT_objectives";
	_info[1].image_id = "TUT_controls";
	_info[2].image_id = "TUT_rules";
	_info[3].image_id = "TUT_actions";

	_page = 0;

}
Menu::~Menu(){
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

