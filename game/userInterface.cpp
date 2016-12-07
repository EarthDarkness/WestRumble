#include "userInterface.h"
#include "NetProtocol.h"
#include "../engine/translator.h"
#include "Props.h"


userInterface::userInterface(){
	_stg = NULL;
	_tileMap = NULL;
	_teams[0] = NULL;
	_teams[1] = NULL;

	_selected = NULL;
	_target = NULL;
	_tracked = NULL;
	_font = NULL;

	_player = 0;
}
userInterface::~userInterface(){
}



void userInterface::pushOvelay(int x, int y, int ol){
	_overlay.push_back(Actors());
	_overlay.back().getAnimation().init(UI_ovelay);
	_overlay.back().setPos(x,y);
	_overlay.back().setState(ol);
}

void userInterface::setFont(font * f){
	_font = f;
}

void userInterface::init(Stage* stg){
	_stg = stg;
	_tileMap = &(_stg->_tileMap);
	_teams[0] = &(_stg->getTeam(0));
	_teams[1] = &(_stg->getTeam(1));
	_ohi.getAnimation().init(UI_action);
}


matMN<Tile>& userInterface::getTileMap(){
	return _tileMap->_map;
}
bool userInterface::areEnemies(Character* c1, Character* c2){
	if(_teams[0]->isMember(c1)>-1 && _teams[0]->isMember(c2)>-1)
		return false;
	if(_teams[1]->isMember(c1)>-1 && _teams[1]->isMember(c2)>-1)
		return false;
	return true;
}


Actors* userInterface::getActorAt(int x, int y){
	if (x<0 || y<0 || x >= _tileMap->_map.width() || y >= _tileMap->_map.height())
		return NULL;
	return _tileMap->_map.at(x, y).actor;
}
Actors* userInterface::getOverlayAt(int x, int y){
	for(int i=0;i<_overlay.size();++i)
		if(_overlay[i].getX() == x && _overlay[i].getY() == y)
			return &_overlay[i];
	return NULL;
}


void userInterface::update(int xm, int ym, int player, int val){
	Actors* clicked = getActorAt(xm,ym);
	_tracked = clicked;

	if((_stg->_turn%2)==player){
		if(val == 1){
			char buf[512] = "\0";

			WrpEncodeEndTurn(buf);

			if(strlen(buf) > 0)
				_actionMsg.push(string(buf));

			_selected = NULL;
			clearOverlays();
			clearCommands();

			return;
		}
		bool ovlhit = false;
		if(getOverlayAt(xm,ym) != NULL)
			if(getOverlayAt(xm,ym)->_sprite.getState() != ANIM_NONE)
				ovlhit = true;

		if(ovlhit){
			updateCommand(xm,ym,player);
			setOHI(ANIM_NONE,0,0);
		}else if(clicked == NULL){
			_selected = NULL;
			setOHI(ANIM_NONE,0,0);
			clearOverlays();
			clearCommands();
		}else if(clicked == _selected){
			if(_commands.empty()){
				//_selected = NULL;
				setOHI(ANIM_NONE,0,0);
				clearOverlays();
				fillCommands(player);
			}else{
				_commands.pop();
			}
			initCommand();
		}else if(_teams[player]->checkSelected(clicked->getCharacter()) != -1){
			clearOverlays();
			_selected = clicked;
			fillCommands(player);
			initCommand();
		}
	}else{

		//visualization


	}
	
}


void userInterface::updateCommand(int xm, int ym, int player){
	/*if(_commands.empty())
		return;

	if(_selected == getActorAt(xm,ym)){
		_commands.pop();
		return;
	}*/
	_player = player;

	Actors* mark = getOverlayAt(xm,ym);
	if(mark == NULL)
		return;

	Character* chr = _selected->getCharacter();


	Team* curteam = _teams[player];
	int pos = curteam->checkSelected(chr);

	int act = _commands.front();

	char buf[512] = "\0";
	int pid = _teams[player]->isMember(chr);

	if(act == ACTIONMOVE){
		WrpEncodeMove(buf,pid,xm,ym);
	}else if(act == ACTIONBOMB){
		WrpEncodeDynamite(buf,pid,xm,ym);
	}else if (act == ACTIONGUNFIRE){
		int dx = xm-chr->getX();
		int dy = ym-chr->getY();

		int dir = 0;
		if(dx == 1){ 
			dir = WRP_RIGHT;
		}else if(dx == -1){ 
			dir = WRP_LEFT;
		}else if(dy == 1){ 
			dir = WRP_DOWN;
		}else if(dy == -1){ 
			dir = WRP_UP;
		}

		WrpEncodeShot(buf,pid,dir);
	}else if (act == ACTIONTIMER_UP){
		WrpEncodeTimeUp(buf,pid,xm,ym);
	}else if (act == ACTIONTIMER_DOWN){
		WrpEncodeTimeDown(buf,pid,xm,ym);		
	}else if (act == ACTIONRELAUNCHSELECT){
		_target = _tileMap->_map.at(xm,ym).actor;
		placeCommands(ACTIONRELAUNCH);
		_commands.pop();
		return;
	}else if (act == ACTIONRELAUNCH){
		WrpEncodeThrow(buf,pid,_target->getX(),_target->getY(),xm,ym);
		_target = NULL;
	}else if (act == ACTIONBARRIER){
		WrpEncodeBarrel(buf,pid,xm,ym);
	}else if (act == ACTIONSTUN){
		WrpEncodeRope(buf,pid,xm,ym);		
	}else if(act == ACTIONBOMBDETONATOR){
		WrpEncodeDetonate(buf,pid);
	}

	if(buf[0] > '\0')
		_actionMsg.push(string(buf,buf[0]));
	

	_selected = NULL;
	clearOverlays();
	clearCommands();
}
void userInterface::initCommand(){
	if(_commands.empty()){
		clearOverlays();
		setOHI(ANIM_NONE,0,0);
		return;
	}

	int act = _commands.front();

	Character* chr = static_cast<Character*>(_selected);

	markEntry(chr);

	if(act == ACTIONMOVE){
		markWalk(chr);
		setOHI(ANIM_MOVE,chr->getX(),chr->getY());
	}else if(act == ACTIONBOMB){
		markBomb(chr);
		setOHI(ANIM_BOMB,chr->getX(),chr->getY());
	}else if(act == ACTIONGUNFIRE){
		markGunFire(chr);
		setOHI(ANIM_SHOT,chr->getX(),chr->getY());
	}else if(act == ACTIONTIMER_UP){
		markTimerUp(chr);
		setOHI(ANIM_TIMERUP,chr->getX(),chr->getY());
	}else if(act == ACTIONTIMER_DOWN){
		markTimerDown(chr);
		setOHI(ANIM_TIMERDOWN,chr->getX(),chr->getY());
	}else if(act == ACTIONRELAUNCHSELECT){
		markThrow(chr);
		setOHI(ANIM_GRAB,chr->getX(),chr->getY());
	}else if(act == ACTIONRELAUNCH){
		markBomb(chr);
		setOHI(ANIM_LAUNCH,chr->getX(),chr->getY());
	}else if(act == ACTIONBARRIER){
		markBarrel(chr);
		setOHI(ANIM_BARREL,chr->getX(),chr->getY());
	}else if(act == ACTIONSTUN){
		markRope(chr);
		setOHI(ANIM_STUN,chr->getX(),chr->getY());
	}else if(act == ACTIONBOMBDETONATOR){
		markDetonator(chr);
		setOHI(ANIM_DETOANTE,chr->getX(),chr->getY());
	}
	
}
void userInterface::fillCommands(int player){
	clearCommands();

	if(_selected == NULL)
		return;

	if(_selected->getClass() != ACTOR_CHAR)
		return;

	Character* chr = _selected->getCharacter();
	Team* curteam = _teams[player];

	int pos = curteam->checkSelected(chr);
	if(pos == -1)
		return;

	if(curteam->actions[pos] == CHAR_END)
		return;
	//if(curteam->actions[pos] < CHAR_MOVED)
	if(curteam->_state[pos].canUse(ACTIONMOVE))
		_commands.push(ACTIONMOVE);
	if(chr->haveBomb())
		_commands.push(ACTIONBOMB);


	//checar se nao esta em cooldown
	if(curteam->_state[pos].canUse(ACTIONGUNFIRE))
		_commands.push(ACTIONGUNFIRE);


	for(int i = POWUP_ACTIVE + 1; i < POWUP_END; i++){
		if (chr->queryPowerUp(i)){
			if(!curteam->_state[pos].canUse(i))
				return;

			if (i == POWUP_BOMB_TIMER){
				_commands.push(ACTIONTIMER_UP);
				_commands.push(ACTIONTIMER_DOWN);
			} else if( i == POWUP_BOMB_RELAUNCH){
				_commands.push(ACTIONRELAUNCHSELECT);
			} else if (i == POWUP_BARRIER){
				_commands.push(ACTIONBARRIER);
			} else if (i == POWUP_STUN){
				_commands.push(ACTIONSTUN);
			}else if ( i == POWUP_DETONATOR){
				_commands.push(ACTIONBOMBDETONATOR);
			}
		}
	}
}
void userInterface::clearCommands(){
	while(!_commands.empty())
		_commands.pop();
}
void userInterface::placeCommands(int cmd){
	int f = _commands.front();
	_commands.push(_commands.front());
	_commands.pop();
	_commands.push(cmd);

	while(_commands.front() != f){
		_commands.push(_commands.front());
		_commands.pop();
	}

}

void userInterface::setOHI(int name, int x, int y){
	_ohi.getAnimation().setState(name);
	_ohi.setPos(x,y);
}


void userInterface::clearOverlays(){
	_overlay.clear();
}

void userInterface::markEntry(Actors* entry){
	clearOverlays();

	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	pushOvelay(entry->getX(),entry->getY(),ANIM_NONE);
}
void userInterface::markWalk(Character* entry){
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -entry->getSpeed();j<=entry->getSpeed();++j){
		for(int i = -entry->getSpeed();i<=entry->getSpeed();++i){

			if(abs(j)+abs(i) > entry->getSpeed())
				continue;

			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf != NULL){
				if(buf->getClass() != ACTOR_POWUP){
					continue;
				}
			}

			pushOvelay(xx,yy,ANIM_MOVE);
		}
	}
}
void userInterface::markBomb(Character* entry){
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -2;j<=2;++j){
		for(int i = -2;i<=2;++i){

			if(abs(j)+abs(i) > 2)
				continue;

			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf != NULL)
				continue;
			


			pushOvelay(xx,yy,ANIM_BOMB);
		}
	}
}
void userInterface::markGunFire(Character* entry){
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -1;j<=1;++j){
		for(int i = -1;i<=1;++i){
			if(i==0&&j==0)
				continue;
			if(i!=0&&j!=0)
				continue;

			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(!_tileMap->inBound(xx,yy))
				continue;


			pushOvelay(xx,yy,ANIM_SHOT);
		}
	}
}
void userInterface::markTimerUp(Character* entry){
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;
	int range = 4;
	for(int j = -range;j<=range;++j){
		for(int i = -range;i<=range;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;
			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;
			


			pushOvelay(xx,yy,ANIM_TIMERUP);
		}
	}
}
void userInterface::markTimerDown(Character* entry){
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;
	int range = 4;
	for(int j = -range;j<=range;++j){
		for(int i = -range;i<=range;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;
			


			pushOvelay(xx,yy,ANIM_TIMERDOWN);
		}
	}
}
void userInterface::markThrow(Character* entry){//TODO sprite
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -1;j<=1;++j){
		for(int i = -1;i<=1;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;


			pushOvelay(xx,yy,ANIM_LAUNCH);
		}
	}
}
void userInterface::markDetonator(Character* entry){
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = 0;j<getTileMap().height();++j){
		for(int i = 0;i<getTileMap().width();++i){
			int xx = i;
			int yy = j;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;

			if(buf->getBomb()->getOwner() != _teams[_player]->isMember(_selected->getCharacter()))
				continue;


			pushOvelay(xx,yy,ANIM_DETOANTE);
		}
	}
}
void userInterface::markBarrel(Character* entry){//TODO sprite
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -2;j<=2;++j){
		for(int i = -2;i<=2;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf != NULL)
				continue;
			


			pushOvelay(xx,yy,ANIM_BARREL);
		}
	}
}
void userInterface::markRope(Character* entry){//TODO sprite
	if(!_tileMap->inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -2;j<=2;++j){
		for(int i = -2;i<=2;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap->getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap->getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_CHAR)
				continue;
			if(!areEnemies(static_cast<Character*>(entry),static_cast<Character*>(buf)))
				continue;


			pushOvelay(xx,yy,ANIM_STUN);
		}
	}
}


void userInterface::renderOverlay(renderer& ren, camera& cam){
	for(int i=0;i<_overlay.size();++i)
		ren.renderActorInMapCenter(_overlay[i], cam);//render actors in tile
}
void userInterface::renderIcons(renderer& ren, camera& cam){
	ren.renderActorInMapCenter(_ohi,cam,-15,-75);
	if(_tracked){
		int x = -10;
		int y = -10;
		if(_tracked->getClass() == ACTOR_CHAR){
			ren.renderSprite(IMG_GFX::bomb,x,y);
			ren.renderText(i_to_str(_tracked->getCharacter()->getBombs()).c_str(),*_font,x+70,y+22);
			ren.renderSprite(IMG_GFX::boot,x,y+40);
			ren.renderText(i_to_str(_tracked->getCharacter()->getSpeed()).c_str(),*_font,x+70,y+62);
			ren.renderSprite(IMG_GFX::gunpowder,x,y+80);	
			ren.renderText(i_to_str(_tracked->getCharacter()->getFire()).c_str(),*_font,x+70,y+102);
		}else if(_tracked->getClass() == ACTOR_BOMB){
			ren.renderSprite(IMG_GFX::gunpowder,x,y);
			if(_tracked->getBomb()->getTurn()>0){
				ren.renderSprite(IMG_GFX::timer,x,y+40);
				ren.renderText(i_to_str(_tracked->getBomb()->getTurn()).c_str(),*_font,x+70,y+62);
			}
		}
	}

}
void userInterface::clear(){
	_overlay.clear();
}