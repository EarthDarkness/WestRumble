#include "Game.h"
#include "block.h"
#include "NetProtocol.h"
#include "Props.h"

Game::Game(){
	selected = NULL;

	//stateActionCharacter = ACTIONNULL;
	special = NULL;
	player = 0;
	field = false;
	width_screen = 1024;
	height_screen = 768;
	title = "West Rumble!!!";
}
Game::~Game(){

}

void Game::init(){
	state = STATEMENU;

	engine.init();

	SDL_DisplayMode displayMode;
	if( SDL_GetCurrentDisplayMode(0, &displayMode)== 0){
		width_screen = displayMode.w;
		height_screen = displayMode.h;
	}
	engine.createWindow(title, width_screen, height_screen);

	SDL_GL_GetDrawableSize(engine.getWindow(),&width_screen,&height_screen);

	_cam.setCam(&stage.getCamera());
	_cam.setSpeed(1.0f/10.0f);

	loadResources();

	setGUI();

	stage.init(&engine._font);
	shop.setGUI(width_screen,height_screen);

	reset();


}
void Game::setGUI(){
	int dpi = 20;
	int ss = (height_screen<width_screen?height_screen:width_screen)/dpi;
	int space = ss/4;
	int dist = ss*3;
	int sw = width_screen-2*(space+ss+dist);
	int sh = height_screen-2*(space+ss+dist);
	

	button_end_turn.image_id = IMG_UI::endturn;
	button_end_turn.setPos(width_screen-dist-ss/2,height_screen-dist-ss/2,dist,dist);

	button_move_left.image_id = IMG_UI::move;
	button_move_right.image_id = IMG_UI::move;
	button_move_up.image_id = IMG_UI::move;
	button_move_down.image_id = IMG_UI::move;

	button_move_left.setPos(space,space+ss+dist,ss,sh);
	button_move_right.setPos(width_screen-space-ss,space+ss+dist,ss,sh);
	button_move_up.setPos(space+ss+dist,space,sw,ss);
	button_move_down.setPos(space+ss+dist,height_screen-space-ss,sw,ss);

	//menu.start.setPos(width_screen/2-100/2,((height_screen/2-50/2)/2)*3,100,50);
	menu.setGUI(width_screen,height_screen);
}

void Game::loadResources(){
	engine._res.loadMusic("resources/lilywest.wav","BGM");

	loadProps(engine._res);

	engine.loadFont();
	shop.load(engine._res);

	_speed = IMG_GFX::boot;
	_bombs = IMG_GFX::bomb;
	_fire = IMG_GFX::gunpowder;
	_turn = IMG_GFX::timer;
		
}

void Game::reset(){
	player =0 ;
	field = false;
	endGame = false;
	A.reset();
	B.reset();

	player = 0;

	menu._state = MAINMENU;
	state = STATEMENU;
	cout << "END GAME!!!" << endl;

	stage.clear();
}

void Game::update(){

	engine.sortEvents();

	if(engine._winChanged){
		SDL_GL_GetDrawableSize(engine.getWindow(),&width_screen,&height_screen);
		shop.setGUI(width_screen,height_screen);
		setGUI();
		if(state == STATEGAME)
			centerTeam(player);
		engine._winChanged = false;
	}

	if(engine._back){
		if(state == STATEGAME){
			reset();
		}else{
			engine.quit();
		}
		engine._back = false;
	}

	switch (state)
	{
	case STATEMENU:{
		updateMenu();
		break;
				   }
	case STATESHOP_A:{
		updateShop();
		//cout << "Shop A" << endl;

		if (shop.isDone()){
			state = STATESHOP_B;

			shop.init(&B,1);
		}
		break;
					 }
	case STATESHOP_B:{
		updateShop();
		//cout << "Shop B" << endl;

		if (shop.isDone()){
			state = STATEGAME;
			//engine._render.playMusic("BGM",true,false);
			_ui.init(&stage);
		}
		break;
					 }
	case STATEGAME:{
		if (endGame)
			//reset();
			state = STATEEND;
		else
			updateStage();
		break;
				   }
	case STATEEND:{

		updateEnd();
		break;
					}
	}
}
void Game::updateMenu(){

	if(menu._state == MAINMENU){
		if(engine._input.isPress()){
			if(menu._play.checkCollision(engine._input.getX(), engine._input.getY())){
				menu._state = STAGEMENU;
			}else if(menu._tutorial.checkCollision(engine._input.getX(), engine._input.getY())){
				menu._state = TUTORIALMENU;
				menu._page = 0;
			}
		}
	}else if(menu._state == STAGEMENU){
		cout << "Select Stage" << endl;

		if (stage.loadStage("resources/stage/stage1.txt", engine._res)){
			cout << "stage1.txt loaded" << endl;
		}

		stage.start(&A, &B);
		centerTeam(0);
		state = STATESHOP_A;

		shop.init(&A,0);
	}else if(menu._state == TUTORIALMENU){
		if(engine._input.isPress()){
			if(menu._next.checkCollision(engine._input.getX(), engine._input.getY())){
				++menu._page;
			}else if(menu._prev.checkCollision(engine._input.getX(), engine._input.getY())){
				--menu._page;
			}else if(menu._back.checkCollision(engine._input.getX(), engine._input.getY())){
				menu._state = MAINMENU;
			}
			menu._page = (menu._page+4)%4;
		}
	}

}
void Game::updateShop(){
	shop.update(engine);

}
void Game::updateStage(){
	if(stage.waitForVFX())
		return;

	if(!_cam.isDone())
		_cam.update();

	//test no move left
	/*if(!field){//if player turn
		field = true;
		for(int i=0;i<5;++i)
			if(stage.getTeam(player).getCharacter(i).getAlive())
				if(stage.getTeam(player).actions[i] != CHAR_END)
					field = false;
	}*/

	if(!field)
		turnPlayer();

	while(!_ui._actionMsg.empty()){
		_actionMsg.push(_ui._actionMsg.front());
		_ui._actionMsg.pop();
	}
	proccessMessages();

	if(field)//one per turn end
		turnField();


	//move buttons
	if(engine._input.isDown()){
		if(button_move_right.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(-10, 0);
			return;
		}else if(button_move_left.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(10, 0);
			return;
		}else if(button_move_up.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(0, 10);
			return;
		}else if(button_move_down.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(0, -10);
			return;
		}
	}
	
	
	
	
	//turn button
	//if(engine._input.isPress()){
	//	if(button_end_turn.checkCollision(engine._input.getX(), engine._input.getY())){
	//		if(!field){
	//			field = true;
	//			stage.clearOverlays();
	//			while(!_selectedActions.empty())
	//				_selectedActions.pop();
	//		}
	//		
	//			//return;
	//		/*stage.clearOverlays();
	//		selected = NULL;
	//		stage.getTeam(player).beginTurn();
	//		centerTeam(player);*/
	//	}
	//	if(!field)//player turn
	//		turnPlayer();
	//}

	//if(field)//bomb turn
	//	turnField();
	

	


	//turno da bomba
	//if(player == 2){
	//	//dem
	//	vector<Actors*> hits;
	//	//calc bomb to explode
	//	for (list<bomb>::iterator it = stage.bombs.begin(); it != stage.bombs.end(); it++){
	//		if (it->getClass() == ACTOR_BOMB){
	//			bomb* ptr = &*it;
	//			if(ptr->getTurn() != -1){
	//				ptr->addTurn(-1);
	//				if(ptr->getTurn() < 1){
	//					hits.push_back(ptr);
	//				}
	//			}
	//		}
	//	}
	//	//calc gunfire
	//	for(int i=0;i<2;++i){
	//		Team* tbuf = &stage.getTeam(i);
	//		for(int j=0;j<5;++j){
	//			if(tbuf->_state[j].getState(ACTIONGUNFIRE) != STATE_FIRE)
	//				continue;
	//			stage.checkGunfire(&tbuf->getCharacter(j),hits);
	//			tbuf->_state[j].setState(ACTIONGUNFIRE,STATE_STAGE2);

	//			if(tbuf->getCharacter(j).direction == GUN_N){
	//				tbuf->getCharacter(j).setState("shot_up");
	//			}else if(tbuf->getCharacter(j).direction == GUN_S){
	//				tbuf->getCharacter(j).setState("shot_down");
	//			}else if(tbuf->getCharacter(j).direction == GUN_W){
	//				tbuf->getCharacter(j).setState("shot_left");
	//			}else if(tbuf->getCharacter(j).direction == GUN_E){
	//				tbuf->getCharacter(j).setState("shot_right");
	//			}
	//		}
	//	}
	//	//calc chain explosion
	//	for(int i=0;i<hits.size();++i){
	//		if(hits[i]->getClass() == ACTOR_BOMB)
	//			stage.checkExplosion(static_cast<bomb*>(hits[i]),hits);
	//	}
	//	//apply damage
	//	while(!hits.empty()){
	//		if(hits.back()->getClass() == ACTOR_BOMB){
	//			//stage.bombs.erase(find(stage.bombs.begin(),stage.bombs.end(),*(static_cast<bomb*>(hits.back()))));
	//			bomb* b = static_cast<bomb*>(hits.back());
	//			if(b->getOwner() != NULL)
	//				b->getOwner()->RemoveEntry(b->getX(),b->getY());
	//			stage.getTileMap().at(hits.back()->getX(),hits.back()->getY()).actor = NULL;
	//			for(list<bomb>::iterator it=stage.bombs.begin();it!=stage.bombs.end();++it){
	//				if(&*it == hits.back()){
	//					stage.bombs.erase(it);
	//					break;
	//				}
	//			}

	//		}else if(hits.back()->getClass() == ACTOR_CHAR){
	//			stage.hitChar(static_cast<Character*>(hits.back()));
	//		}else if(hits.back()->getClass() == ACTOR_POWUP){
	//			stage.hitPowUp(static_cast<PowerUp*>(hits.back()));
	//		}else if(hits.back()->getClass() == ACTOR_BLOCK){
	//			stage.hitBlock(static_cast<block*>(hits.back()));
	//		}
	//		hits.pop_back();
	//	}
	//	cout << "Fim de turno bomba" << endl;

	//	checkEnd();

	//	if(stage.suddenDeath()){
	//		stage.dropBomb(stage.suddenDeath());
	//	}
	//	stage.addTurn();

	//	player = 0;
	//	stage.getTeam(player).beginTurn();
	//	centerTeam(player);
	//	return;
	//}
}
void Game::updateEnd(){
	if (engine._input.isPress()){
		reset();
		state = STATEMENU;
	}
}

void Game::render()
{

	switch (state)
	{
	case STATEMENU:{
		engine._render.renderSprite(IMG_GFX::wood,0,0,width_screen,height_screen);
		renderMenu();
		break;
				   }
	case STATESHOP_A:{
		engine._render.renderSprite(IMG_GFX::wood,0,0,width_screen,height_screen);
		shop.render(engine);
		break;
					 }
	case STATESHOP_B:{
		engine._render.renderSprite(IMG_GFX::wood,0,0,width_screen,height_screen);
		shop.render(engine);
		break;
					 }
	case STATEGAME:{
		renderStage();
		break;
				   }
	case STATEEND:{
		engine._render.renderSprite(IMG_GFX::wood,0,0,width_screen,height_screen);
		renderEnd();
		break;
					}
	}


	engine.flush();
	//SDL_Delay(16);
}
void Game::renderMenu(){
	if(menu._state == MAINMENU){
		engine._render.renderSprite(menu._logo.image_id,menu._logo.rect.x,menu._logo.rect.y,menu._logo.rect.w,menu._logo.rect.h);
			
		engine._render.renderSprite(menu._play.image_id, menu._play.rect.x, menu._play.rect.y, menu._play.rect.w, menu._play.rect.h);
		engine._render.renderSprite(menu._tutorial.image_id, menu._tutorial.rect.x, menu._tutorial.rect.y, menu._tutorial.rect.w, menu._tutorial.rect.h);
		engine._render.renderSprite(menu._credits.image_id, menu._credits.rect.x, menu._credits.rect.y, menu._credits.rect.w, menu._credits.rect.h);
	}else if(menu._state == STAGEMENU){

	}else if(menu._state == TUTORIALMENU){
		engine._render.renderSprite(menu._next.image_id, menu._next.rect.x, menu._next.rect.y, menu._next.rect.w, menu._next.rect.h);
		engine._render.renderSprite(menu._prev.image_id, menu._prev.rect.x, menu._prev.rect.y, menu._prev.rect.w, menu._prev.rect.h);
		engine._render.renderSprite(menu._back.image_id, menu._back.rect.x, menu._back.rect.y, menu._back.rect.w, menu._back.rect.h);

		engine._render.renderSprite(menu._info[menu._page].image_id, menu._info[menu._page].rect.x, menu._info[menu._page].rect.y, menu._info[menu._page].rect.w, menu._info[menu._page].rect.h);
	}
	
}
void Game::renderStage()
{
	stage.renderMap(engine._render);
	_ui.renderOverlay(engine._render,stage.getCamera());
	stage.renderActors(engine._render);
	_ui.renderIcons(engine._render,stage.getCamera());



	//render button right move
	engine._render.renderSprite(button_move_right.image_id, button_move_right.rect.x, button_move_right.rect.y, button_move_right.rect.w, button_move_right.rect.h);

	//render button left move
	engine._render.renderSprite(button_move_left.image_id, button_move_left.rect.x, button_move_left.rect.y, button_move_left.rect.w, button_move_left.rect.h);

	//render button up move
	engine._render.renderSprite(button_move_up.image_id, button_move_up.rect.x, button_move_up.rect.y, button_move_up.rect.w, button_move_up.rect.h);

	//render button down move
	engine._render.renderSprite(button_move_down.image_id, button_move_down.rect.x, button_move_down.rect.y, button_move_down.rect.w, button_move_down.rect.h);

	//render button end turn
	engine._render.renderSprite(button_end_turn.image_id, button_end_turn.rect.x, button_end_turn.rect.y, button_end_turn.rect.w, button_end_turn.rect.h);


	//render status
	if(visualized != NULL){
		if(visualized->getClass() == ACTOR_CHAR){
			//render speed
			engine._render.renderSprite(_speed, -15, -15);
			engine._render.renderText(i_to_str(((Character*)visualized)->getSpeed()).c_str(), engine._font, 0 + 25 + 20, 0);
			//render bomb
			engine._render.renderSprite(_bombs, -15, 50-15);
			engine._render.renderText(i_to_str(((Character*)visualized)->getBombs()).c_str(), engine._font, 0 + 25 + 20, 50);
			//render fire
			engine._render.renderSprite(_fire, -15, 100-15);
			engine._render.renderText(i_to_str(((Character*)visualized)->getFire()).c_str(), engine._font, 0 + 25 + 20, 100);
		} else if(visualized->getClass() == ACTOR_BOMB){

			//render fire
			engine._render.renderSprite(_fire, -15, -15);
			engine._render.renderText(i_to_str(((bomb*)visualized)->getFire()).c_str(), engine._font, 0 + 25 + 20, 0);

			//render turn
			engine._render.renderSprite(_turn, -15, 50-15);
			engine._render.renderText(i_to_str(((bomb*)visualized)->getTurn()).c_str(), engine._font, 0 + 25 + 20, 50);

		}

	}


	//render player
	if (player == 0){
		engine._render.renderSprite(IMG_GFX::win1, 0 + 20 + 200, 0);
	}
	else if (player == 1){
		engine._render.renderSprite(IMG_GFX::win2, 0 + 20 + 200, 0);
	}
	else if (player == 2)
	{

	}


}
void Game::renderEnd(){
	if(winner == 0){
		engine._render.renderSprite(IMG_GFX::win1, width_screen/2 - 50, height_screen/2 - 20);
	}else if(winner == 1){
		engine._render.renderSprite(IMG_GFX::win2, width_screen/2 - 50, height_screen/2 - 20);
	}else{
		engine._render.renderSprite(IMG_GFX::tie, width_screen/2 - 50, height_screen/2 - 20);
	}

}


void Game::proccessMessages(){
	while(!_actionMsg.empty()){
		const char* msg = _actionMsg.front().c_str();
		Team* curteam = &stage.getTeam(player);

		if(msg[0] == WRP_END_TURN){
			field = true;

		}else if(msg[0] == WRP_MOVE){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeMove(msg,pid,xx,yy);

			Character* act = &(curteam->getCharacter(pid));

			Actors* tgt = stage.getActorAt(xx,yy);
			if(tgt != NULL){
				if(tgt->getClass() == ACTOR_POWUP) act->AddPowerUp((PowerUp*)tgt);
			}

			stage.moveActor(act->getX(), act->getY(), xx, yy);//TODO validate movment

			curteam->actions[pid] = CHAR_MOVED;
			curteam->_state[pid].addCooldown(ACTIONMOVE,1);

		}else if(msg[0] == WRP_DYNAMITE){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeDynamite(msg,pid,xx,yy);

			Character* act = &(curteam->getCharacter(pid));

			bomb b;
			b.init(act->getFire(),stage.bombs.size(),player,pid);
			if(act->queryPowerUp(POWUP_DETONATOR)) b.setTurn(-1);

			stage.bombs.push_back(b);
			//act->AddEntry((bomb*)&stage.bombs.back());
			act->AddDynamite(stage.bombs.size());
			stage.instantiateActor(&stage.bombs.back(), xx, yy);

			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_SHOT){
			int pid = 0;
			int dir = 0;

			WrpDecodeShot(msg,pid,dir);

			Character* act = &(curteam->getCharacter(pid));

			if(dir == WRP_RIGHT){
				act->direction = GUN_E;
				act->setState(ANIM_READY_R);
			}else if(dir == WRP_LEFT){
				act->direction = GUN_W;
				act->setState(ANIM_READY_L);
			}else if(dir == WRP_DOWN){
				act->direction = GUN_S;
				act->setState(ANIM_READY_D);
			}else if(dir == WRP_UP){
				act->direction = GUN_N;
				act->setState(ANIM_READY_U);
			}

			act->shot = false;
			curteam->_state[pid].addCooldown(ACTIONGUNFIRE,3);
			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_TIME_UP){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeTimeUp(msg,pid,xx,yy);

			bomb* b = (bomb*)stage.getActorAt(xx, yy);
			if(b != NULL) b->addTurn(1);

			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_TIME_DOWN){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeTimeDown(msg,pid,xx,yy);

			bomb* b = (bomb*)stage.getActorAt(xx, yy);
			if(b != NULL) b->addTurn(-1);

			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_THROW){
			int pid = 0;
			int xi = 0;
			int yi = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeThrow(msg,pid,xi,yi,xx,yy);

			bomb* b = (bomb*)stage.getActorAt(xi, yi);
			if(b != NULL){
				b->setPos(xx, yy);
				stage.getTileMap().at(xx, yy).actor = b;
				stage.getTileMap().at(xi, yi).actor = NULL;
			}

			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_BARREL){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeBarrel(msg,pid,xx,yy);

			stage.blocks.push_back(block());
			stage.instantiateActor(&stage.blocks.back(), xx, yy);

			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_ROPE){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeRope(msg,pid,xx,yy);

			Character* tgt = (Character*)stage.getActorAt(xx,yy);

			if(tgt != NULL){
				if(tgt->getClass() == ACTOR_CHAR){
					Team* opo = &stage.getTeam((player+1)%2);
					opo->_state[opo->checkSelected(tgt)].addCooldown(ACTIONMOVE,2);
				}
			}
			curteam->_state[pid].addCooldown(ACTIONSTUN,1);
			curteam->actions[pid] = CHAR_END;

		}else if(msg[0] == WRP_DETONATE){
			int pid = 0;

			WrpDecodeDetonate(msg,pid);

			Character* act = &(curteam->getCharacter(pid));

			act->detonate();
			curteam->actions[pid] = CHAR_END;
		}
		_actionMsg.pop();
	}
}


void Game::centerAt(Actors* entry){
	int xb;
	int yb;
	matToWin(entry->getX(),entry->getY(),0,0,stage.getCamera().getScale(),xb,yb);
	_cam.goTo(-xb+width_screen/2-stage.getCamera().getScale()/2,-yb+height_screen/2-stage.getCamera().getScale()/4);
}
void Game::centerTeam(int t){
	Team* tt = &stage.getTeam(t);
	int xx = 0;
	int yy = 0;
	int c=0;
	for(int i=0;i<5;++i){
		if(tt->getCharacter(i).getAlive()){
			++c;
			int xb;
			int yb;
			matToWin(tt->getCharacter(i).getX(),tt->getCharacter(i).getY(),0,0,stage.getCamera().getScale(),xb,yb);
			xx+=xb;
			yy+=yb;
		}
	}
	if(c!=0){
		xx/=c;
		yy/=c;
	}
	_cam.goTo(-xx+width_screen/2-stage.getCamera().getScale()/2,-yy+height_screen/2-stage.getCamera().getScale()/4);
}

void Game::calcActions(){
	while(!_selectedActions.empty())
		_selectedActions.pop();
	
	if(selected == NULL)
		return;

	if(selected->getClass() != ACTOR_CHAR)
		return;

	Character* current = (Character*)selected;
	Team* curteam = &stage.getTeam(player);

	int pos = curteam->checkSelected(current);
	if(pos == -1)
		return;

	if(curteam->actions[pos] == CHAR_END)
		return;
	//if(curteam->actions[pos] < CHAR_MOVED)
	if(curteam->_state[pos].canUse(ACTIONMOVE))
		_selectedActions.push(ACTIONMOVE);
	if(current->haveBomb())
		_selectedActions.push(ACTIONBOMB);

	
	//checar se nao esta em cooldown
	if(curteam->_state[pos].canUse(ACTIONGUNFIRE))
		_selectedActions.push(ACTIONGUNFIRE);
		

	for(int i = POWUP_ACTIVE + 1; i < POWUP_END; i++){
		if (current->queryPowerUp(i)){
			if(!curteam->_state[pos].canUse(i))
				return;

			if (i == POWUP_BOMB_TIMER){
				_selectedActions.push(ACTIONTIMER_UP);
				_selectedActions.push(ACTIONTIMER_DOWN);
			} else if( i == POWUP_BOMB_RELAUNCH){
				_selectedActions.push(ACTIONRELAUNCHSELECT);
			} else if (i == POWUP_BARRIER){
				_selectedActions.push(ACTIONBARRIER);
			} else if (i == POWUP_STUN){
				_selectedActions.push(ACTIONSTUN);
			}else if ( i == POWUP_DETONATOR){
				_selectedActions.push(ACTIONBOMBDETONATOR);
				//_selectedActions.push(ACTIONEXPLODEDETONATOR);
			}
		}
	}


}
void Game::initAction(){
	if(_selectedActions.empty()){
		//stage.clearSelectedTiles();
		//stage.clearOverlays();
		stage.setAction("NULL",0,0);
		return;
	}
	
	int act = _selectedActions.front();

	if(act == ACTIONMOVE){
		//stage.markWalk(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_move",selected->getX(),selected->getY());
	}else if(act == ACTIONBOMB){
		//stage.markBomb(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_bomb",selected->getX(),selected->getY());
	}else if(act == ACTIONGUNFIRE){
		//stage.markGunFire(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_shot",selected->getX(),selected->getY());
	}else if(act == ACTIONTIMER_UP){
		//stage.markTimerUp(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_timerup",selected->getX(),selected->getY());
	}else if(act == ACTIONTIMER_DOWN){
		//stage.markTimerDown(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_timerdown",selected->getX(),selected->getY());
	}else if(act == ACTIONRELAUNCHSELECT){
		//stage.markThrow(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_grab",selected->getX(),selected->getY());
	}else if(act == ACTIONRELAUNCH){
		//stage.markBomb(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_throw",selected->getX(),selected->getY());
	}else if(act == ACTIONBARRIER){
		//stage.markBarrel(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_barrel",selected->getX(),selected->getY());
	}else if(act == ACTIONSTUN){
		//stage.markRope(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_stun",selected->getX(),selected->getY());
	}else if(act == ACTIONBOMBDETONATOR){
		//stage.markDetonator(static_cast<Character*>(selected));
		stage.setAction("GFX_ACT_detonate",selected->getX(),selected->getY());
	}
	//stage.markEntry(selected);
}
void Game::updateAction(int x, int y){
	//FROM HERE //change when endshot isDone
	/*for(int j=0;j<2;++j){
		for(int i=0;i<5;++i){
			if(!stage.getTeam(j).getCharacter(i).getAnimation().isDone(stage.getTeam(j).getCharacter(i).getAnimation().getState()))
				continue;
			if(strcmp(stage.getTeam(j).getCharacter(i).getAnimation().getState(),"endshot")==0)
				continue;
				if(stage.getTeam(j)._state[i].getState(ACTIONGUNFIRE) == 1){
					if(stage.getTeam(j).getCharacter(i).direction == GUN_E){
						stage.getTeam(j).getCharacter(i).setState("idle_right");
					}else if(stage.getTeam(j).getCharacter(i).direction == GUN_W){
						stage.getTeam(j).getCharacter(i).setState("idle_left");
					}else if(stage.getTeam(j).getCharacter(i).direction == GUN_N){
						stage.getTeam(j).getCharacter(i).setState("idle_up");
					}else if(stage.getTeam(j).getCharacter(i).direction == GUN_S){
						stage.getTeam(j).getCharacter(i).setState("idle_down");
					}
				}
			}
		}
	}*/

	proccessMessages();


	if(_selectedActions.empty())
		return;

	if(selected == stage.getActorAt(x,y)){
		_selectedActions.pop();
		return;
	}
	//Actors* mark = stage.getOverlayAt(x,y);
	//if(mark == NULL)
		//return;
	
	Team* curteam = &stage.getTeam(player);
	int pos = curteam->checkSelected((Character*)selected);

	int act = _selectedActions.front();

	if(act == ACTIONMOVE){
		Actors* buf = stage.getActorAt(x,y);
		bool calc = false;
		if(buf != NULL){
			if(buf->getClass() == ACTOR_POWUP){
				((Character*)selected)->AddPowerUp((PowerUp*)buf);
				calc = true;
				cout << "POWER UP ADICIONADO!" << endl;
			}
		}
		//stage.moveActor(selected->getX(), selected->getY(), mark->getX(), mark->getY());

		centerAt(selected);

		curteam->actions[pos] = CHAR_MOVED;
		curteam->_state[pos].addCooldown(ACTIONMOVE,1);

		if(calc){
			calcActions();
			return;
			//initAction();
		}

	}else if(act == ACTIONBOMB){
		bomb b;
		b.init(((Character*)selected)->getFire(), (Character*)selected);
		if(static_cast<Character*>(selected)->queryPowerUp(POWUP_DETONATOR))
			b.setTurn(-1);

		stage.bombs.push_back(b);
		((Character*)selected)->AddEntry((bomb*)&stage.bombs.back());
		stage.instantiateActor(&stage.bombs.back(), x, y);

		curteam->actions[pos] = CHAR_END;
	}else if (act == ACTIONGUNFIRE){

		int dx = x - ((Character*)selected)->getX();
		int dy = y - ((Character*)selected)->getY();

		if (dx == 1){
			((Character*)selected)->direction = GUN_E;
			((Character*)selected)->setState(ANIM_READY_R);
			cout << "E" << endl;
		}else if (dx == -1){
			((Character*)selected)->direction = GUN_W;
			((Character*)selected)->setState(ANIM_READY_L);
			cout << "W" << endl;
		}else if (dy == 1){
			((Character*)selected)->direction = GUN_S;
			((Character*)selected)->setState(ANIM_READY_D);
			cout << "S" << endl;
		}else if (dy == -1){
			((Character*)selected)->direction = GUN_N;
			((Character*)selected)->setState(ANIM_READY_U);
			cout << "N" << endl;
		}

		((Character*)selected)->shot = false;
		curteam->_state[pos].addCooldown(ACTIONGUNFIRE,3);
		//curteam->_state[pos].addCooldown(ACTIONGUNFIRE,STATE_STAGE1);

		curteam->actions[pos] = CHAR_END;
	}else if (act == ACTIONTIMER_UP){
		((bomb*)stage.getActorAt(x, y))->addTurn(1);
		
		curteam->actions[pos] = CHAR_END;

	}else if (act == ACTIONTIMER_DOWN){
		((bomb*)stage.getActorAt(x, y))->addTurn(-1);
		
		curteam->actions[pos] = CHAR_END;
	}else if (act == ACTIONRELAUNCHSELECT){
		special = (bomb*)stage.getActorAt(x, y);
		_selectedActions.pop();
		_selectedActions.push(ACTIONRELAUNCHSELECT);
		_selectedActions.push(ACTIONRELAUNCH);
		while(_selectedActions.front() != ACTIONRELAUNCHSELECT){
			_selectedActions.push(_selectedActions.front());
			_selectedActions.pop();
		}

	}else if (act == ACTIONRELAUNCH){

		int oldx, oldy;
		oldx = special->getX();
		oldy = special->getY();

		special->setPos(x, y);
		stage.getTileMap().at(x, y).actor = special;
		stage.getTileMap().at(oldx, oldy).actor = NULL;

		curteam->actions[pos] = CHAR_END;

	}else if (act == ACTIONBARRIER){

		block b;

		stage.blocks.push_back(b);
		stage.instantiateActor(&stage.blocks.back(), x, y);
		curteam->actions[pos] = CHAR_END;
		
	}else if (act == ACTIONSTUN){
		Actors* buf = stage.getActorAt(x,y);
		if(buf != NULL){
			if(buf->getClass() == ACTOR_CHAR){
				Team* tbuf;
				if(player==0)
					tbuf = &stage.getTeam(1);
				else
					tbuf = &stage.getTeam(0);
				int pbuf = tbuf->checkSelected((Character*)buf);
					tbuf->_state[pbuf].addCooldown(ACTIONMOVE,2);
			}
		}
		curteam->_state[pos].addCooldown(ACTIONSTUN,1);

		curteam->actions[pos] = CHAR_END;
	}else if(act == ACTIONBOMBDETONATOR){
		((Character*)selected)->detonate();
		curteam->actions[pos] = CHAR_END;
	}else{
		return;
	}
	_selectedActions.pop();
	if(stage.getTeam(player).actions[pos] == CHAR_END)
		while(!_selectedActions.empty())
			_selectedActions.pop();
}

void Game::turnPlayer(){
	/*int mapx, mapy;
	winToMat(engine._input.getX(), engine._input.getY(), stage.getCamera().getX(), stage.getCamera().getY(), stage.getCamera().getScale(), mapx, mapy);

	Actors* clicked = stage.getActorAt(mapx,mapy);
	visualized = clicked;

	if(selected != NULL){
		if(_selectedActions.empty()){
			selected = NULL;
			//stage.clearOverlays();
			stage.setAction("NULL",0,0);
		}else{
			updateAction(mapx,mapy);
		}
	}

	if(stage.getOverlayAt(mapx,mapy) == NULL){//n�o clicou em tile marcado
		if(selected != clicked){
			selected = NULL;
			stage.setAction("NULL",0,0);
			if(clicked != NULL)
			if(clicked->getClass() == ACTOR_CHAR)
			if(stage.getTeam(player).checkSelected((Character*)clicked) != -1){
				selected = clicked;
				centerAt(selected);
			}
			calcActions();
		}
	}
	initAction();*/
	if(engine._input.isPress()){
		int val = 0;
		if(button_end_turn.checkCollision(engine._input.getX(),engine._input.getY()))
			val = 1;
		int xm,ym;
		winToMat(engine._input.getX(), engine._input.getY(), stage.getCamera().getX(), stage.getCamera().getY(), stage.getCamera().getScale(), xm, ym);
		_ui.update(xm,ym,val);
	}
}
void Game::turnField(){
	vector<Actors*> hits;
	//calc bomb to explode
	for (list<bomb>::iterator it = stage.bombs.begin(); it != stage.bombs.end(); it++){
		if (it->getClass() == ACTOR_BOMB){
			bomb* ptr = &*it;
			if(ptr->getTurn() != -1){
				ptr->addTurn(-1);
				if(ptr->getTurn() < 1){
					hits.push_back(ptr);
				}
			}
		}
	}
	//calc gunfire
	//for(int i=0;i<2;++i){
	{
	Team* tbuf = &stage.getTeam(player);
	for(int j=0;j<5;++j){
		if(tbuf->_state[j].getState(ACTIONGUNFIRE) != 2)
			continue;
		if(!stage.getTeam(player).getCharacter(j).getAlive())
			continue;
		stage.checkGunfire(&tbuf->getCharacter(j),hits);
		//tbuf->_state[j].setState(ACTIONGUNFIRE,STATE_STAGE2);

		if(tbuf->getCharacter(j).direction == GUN_N){
			tbuf->getCharacter(j).setState(ANIM_SHOT_U);
		}else if(tbuf->getCharacter(j).direction == GUN_S){
			tbuf->getCharacter(j).setState(ANIM_SHOT_D);
		}else if(tbuf->getCharacter(j).direction == GUN_W){
			tbuf->getCharacter(j).setState(ANIM_SHOT_L);
		}else if(tbuf->getCharacter(j).direction == GUN_E){
			tbuf->getCharacter(j).setState(ANIM_SHOT_R);
		}
	}
	}
	//calc chain explosion
	for(int i=0;i<hits.size();++i){
		if(hits[i]->getClass() == ACTOR_BOMB)
			stage.checkExplosion(static_cast<bomb*>(hits[i]),hits);
	}
	//apply damage
	while(!hits.empty()){
		if(hits.back()->getClass() == ACTOR_BOMB){
			bomb* b = static_cast<bomb*>(hits.back());
			if(b->getOwner() != NULL)
				b->getOwner()->RemoveEntry(b->getX(),b->getY());
			stage.getTileMap().at(hits.back()->getX(),hits.back()->getY()).actor = NULL;
			for(list<bomb>::iterator it=stage.bombs.begin();it!=stage.bombs.end();++it){
				if(&*it == hits.back()){
					stage.bombs.erase(it);
					break;
				}
			}

		}else if(hits.back()->getClass() == ACTOR_CHAR){
			stage.hitChar(static_cast<Character*>(hits.back()));
		}else if(hits.back()->getClass() == ACTOR_POWUP){
			stage.hitPowUp(static_cast<PowerUp*>(hits.back()));
		}else if(hits.back()->getClass() == ACTOR_BLOCK){
			stage.hitBlock(static_cast<block*>(hits.back()));
		}
		hits.pop_back();
	}
	//after gun cooldown
	for(int i=0;i<5;++i){
		if(stage.getTeam(player).getCharacter(i).getAlive()){
			if(stage.getTeam(player)._state[i].getState(ACTIONGUNFIRE) == 1){
				if(stage.getTeam(player).getCharacter(i).direction == GUN_E){
					stage.getTeam(player).getCharacter(i).setState(ANIM_IDLE_R);
				}else if(stage.getTeam(player).getCharacter(i).direction == GUN_W){
					stage.getTeam(player).getCharacter(i).setState(ANIM_IDLE_L);
				}else if(stage.getTeam(player).getCharacter(i).direction == GUN_N){
					stage.getTeam(player).getCharacter(i).setState(ANIM_IDLE_U);
				}else if(stage.getTeam(player).getCharacter(i).direction == GUN_S){
					stage.getTeam(player).getCharacter(i).setState(ANIM_IDLE_D);
				}
			}
		}
	}

	checkEnd();
	if(stage.suddenDeath()){
		stage.dropBomb(stage.suddenDeath());
	}
	stage.addTurn();

	field = false;
	player = (player+1)%2;
	stage.getTeam(player).beginTurn();
	centerTeam(player);

	/*player = 0;
	stage.getTeam(player).beginTurn();
	centerTeam(player);
	return;*/
}

void Game::checkEnd(){
	bool dead[2];
	dead[0] = false;
	dead[1] = false;
	for(int t=0;t<2;++t){
		if(stage.getTeam(t).count < 1){
			endGame = true;
			dead[t] = true;
		}
	}
	if(dead[0] & dead[1])
		winner = -1;
	else if(dead[0])
		winner = 1;
	else if(dead[1])
		winner = 0;
	else
		winner = -1;
	

}

