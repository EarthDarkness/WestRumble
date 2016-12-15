#include "Game.h"
#include "block.h"
#include "NetProtocol.h"
#include "Props.h"


field _log;
string _logFile;

Game::Game(){
	_logFile = "log1.txt";

	selected = NULL;

	//stateActionCharacter = ACTIONNULL;
	special = NULL;
	_player = 0;
	_field = false;
	width_screen = 1024;
	height_screen = 768;
	title = "West Rumble!!!";

	_stageName[0] = "stage1.txt";
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
	
	_movie.init(&stage.getCamera());

	loadResources();

	menu.init();
	menu._com = &engine._com;

	_ui.setFont(&engine._font);

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
	

	button_end_turn._sprite.loadAnim(IMG_UI::endturn);
	button_end_turn.setGUI(width_screen-dist-ss/2,height_screen-dist-ss/2,dist,dist);

	button_move_left._sprite.loadAnim(IMG_UI::move);
	button_move_right._sprite.loadAnim(IMG_UI::move);
	button_move_up._sprite.loadAnim(IMG_UI::move);
	button_move_down._sprite.loadAnim(IMG_UI::move);

	button_move_left.setGUI(space,space+ss+dist,ss,sh);
	button_move_right.setGUI(width_screen-space-ss,space+ss+dist,ss,sh);
	button_move_up.setGUI(space+ss+dist,space,sw,ss);
	button_move_down.setGUI(space+ss+dist,height_screen-space-ss,sw,ss);

	//menu.start.setPos(width_screen/2-100/2,((height_screen/2-50/2)/2)*3,100,50);
	menu.setGUI(width_screen,height_screen);
}

void Game::loadResources(){
	engine._res.loadMusic("resources/lilywest.wav","BGM");
	engine._res.loadSound("resources/gunfire_mix.wav","gunfire");
	engine._res.loadSound("resources/knock.wav","click");
	engine._res.loadSound("resources/explosion.wav","explosion");

	engine._res.loadSound("resources/dynamite.wav","bomb");
	engine._res.loadSound("resources/shot.wav","shot");
	engine._res.loadSound("resources/barrel.wav","barrel");
	engine._res.loadSound("resources/rope.wav","rope");
	engine._res.loadSound("resources/time_up.wav","time_up");
	engine._res.loadSound("resources/time_down.wav","time_down");

	loadProps(engine._res);

	engine.loadFont();
	shop.load(engine._res);

	_speed = IMG_GFX::boot;
	_bombs = IMG_GFX::bomb;
	_fire = IMG_GFX::gunpowder;
	_turn = IMG_GFX::timer;
}

void Game::reset(){
	_player =0 ;
	_field = false;
	endGame = false;
	A.reset();
	B.reset();

	_player = 0;

	_log.save(_logFile.c_str());

	menu._state = MAIN_MENU;
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
			centerTeam(_player);
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
		if(engine._input.isPress()){
			BUTTONCLICKER.clickAt(engine._input.getX(),engine._input.getY());
			engine._render.playSound("click");
		}else{
			BUTTONCLICKER.clickAt();
		}
		menu.udpdate(engine._input.getX(),engine._input.getY());
		if(menu.isDone()){
			string path = "resources/stage/";
			path += _stageName[menu._map];

			if(_stageName[menu._map].empty()){
				//reset
				reset();
				if(menu._net != 0){
					engine._com.close();
				}
				menu.reset();
				state = STATEMENU;
			}else{
				//if(stage.loadStage("resources/stage/stage1.txt", engine._res)){
				if(stage.loadStage(path.c_str(), engine._res)){
					cout << "stage1.txt loaded" << endl;
				}
				_log.init(stage._tileMap);
				_log.load(_logFile.c_str());
				//_log.mult(8);
				//_log.smooth(3);
				//_log.div(2);
				if(menu._net == 0 || menu._net == 1){
					stage.populate();
				}

				stage.start(&A, &B);
				centerTeam(0);


				if(menu._net != 0){
					state = STATEGAME;

					_ui.init(&stage);

					if(menu._net == 1){
						engine._com._hear = 1;
						//sync map
						char buf[LEN] = "\0";
						int len;
						stage.encode(buf,len);
						engine._com.send(buf,len);

					}
				}else{
					state = STATESHOP_A;

					shop.init(&A,0);  
				
				}
			}

		}
		break;
				   }
	case STATESHOP_A:{
		if(engine._input.isPress())
			BUTTONCLICKER.clickAt(engine._input.getX(),engine._input.getY());
		else
			BUTTONCLICKER.clickAt();
		shop.update(engine);

		if (shop.isDone()){
			state = STATESHOP_B;

			shop.init(&B,1);
		}
		break;
					 }
	case STATESHOP_B:{
		if(engine._input.isPress())
			BUTTONCLICKER.clickAt(engine._input.getX(),engine._input.getY());
		else
			BUTTONCLICKER.clickAt();

		shop.update(engine);

		if (shop.isDone()){
			state = STATEGAME;
			engine._render.playMusic("BGM",true,false);
			_ui.init(&stage);
			if(menu._net == 1){
				engine._com._hear = 1;
				//sync map
				char buf[LEN] = "\0";
				int len;
				stage.encode(buf,len);
				engine._com.send(buf,len);
			}
		}
		break;
					 }
	case STATEGAME:{
		if(endGame)
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
void Game::updateStage(){
	if(stage.waitForVFX())
		return;

	

	if(!_cam.isDone())
		_cam.update();

	if(!_movie.isDone()){
		_movie.update();
		return;
	}

	checkEnd();

	bool noClick = false;

	//move buttons
	if(engine._input.isDown()){
		if(button_move_right.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(-10, 0);
			noClick = true;
		}else if(button_move_left.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(10, 0);
			noClick = true;
		}else if(button_move_up.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(0, 10);
			noClick = true;
		}else if(button_move_down.checkCollision(engine._input.getX(), engine._input.getY())){
			stage.getCamera().addPos(0, -10);
			noClick = true;
		}
	}
	if(!noClick)
	if(!_field){
		if(menu._net == 0){
			turnPlayer(_player);
		}else if(menu._net == 1){
			turnPlayer(0);
		}else if(menu._net == 2){
			turnPlayer(1);
		}
	}
		
	while(!_ui._actionMsg.empty()){
		if(menu._net == 0 || menu._net == 1){
			_actionMsg.push(_ui._actionMsg.front());
			_ui._actionMsg.pop();
		}else if(menu._net == 2){
			engine._com.send((void*)(_ui._actionMsg.front().c_str()),_ui._actionMsg.front().size());
			_ui._actionMsg.pop();
		}
	}

	char buf[LEN];

	if(menu._net == 0){
		proccessMessages();
	}else if(menu._net == 1){
		while(engine._com.recv(buf,LEN)>0){
			_actionMsg.push(string(buf,LEN));
		}
		proccessMessages();
	}else if(menu._net == 2){
		if(engine._com.recv(buf,LEN)){
			if(buf[1] == WRP_MOVE){
				_actionMsg.push(string(buf,LEN));
			}else if(buf[1] == WRP_SCENE){
				stage.decode(buf);
				centerTeam(stage._turn%2);
			}
		}
		proccessMessages();
	}

	if(_field)//one per turn end
		turnField();


}
void Game::updateEnd(){
	if (engine._input.isPress()){
		reset();
		if(menu._net != 0){
			engine._com.close();
		}
		menu.reset();
		state = STATEMENU;
	}
}

void Game::render()
{

	switch (state)
	{
	case STATEMENU:{
		engine._render.renderSprite(IMG_GFX::wood,0,0,width_screen,height_screen);
		menu.render(engine._render,engine._font);
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
void Game::renderStage(){

	stage.renderMap(engine._render);
//#define DISPLAY_LOG
#ifdef DISPLAY_LOG
	for(int j=0;j<_log._map.height();++j){
		for(int i=0;i<_log._map.width();++i){
			if(stage._tileMap._map.at(i,j).collision == 0)
				continue;
			_log._sprite.setState(_log._map.at(i,j));
			engine._render.renderSpriteInMap(_log._sprite.get(),stage.getCamera(),i,j);
		}
	}
#endif

	_ui.renderOverlay(engine._render,stage.getCamera());
	stage.renderActors(engine._render);
	_ui.renderIcons(engine._render,stage.getCamera());



	//render button right move
	engine._render.renderSprite(button_move_right.getSprite(), button_move_right._rect);

	//render button left move
	engine._render.renderSprite(button_move_left.getSprite(), button_move_left._rect);

	//render button up move
	engine._render.renderSprite(button_move_up.getSprite(), button_move_up._rect);

	//render button down move
	engine._render.renderSprite(button_move_down.getSprite(), button_move_down._rect);

	//render button end turn
	engine._render.renderSprite(button_end_turn.getSprite(), button_end_turn._rect);

	//if(!_movie.isDone())
		//engine._render.renderSprite(_dummie.getSprite(),_dummie.getX(),_dummie.getY());

	//render status
	if(visualized != NULL){
		if(visualized->getClass() == ACTOR_CHAR){
			//render speed
			engine._render.renderSprite(_speed, -15, -15);
			engine._render.renderText(i_to_str(visualized->getCharacter()->getSpeed()).c_str(), engine._font, 0 + 25 + 20, 0);
			//render bomb
			engine._render.renderSprite(_bombs, -15, 50-15);
			engine._render.renderText(i_to_str(visualized->getCharacter()->getBombs()).c_str(), engine._font, 0 + 25 + 20, 50);
			//render fire
			engine._render.renderSprite(_fire, -15, 100-15);
			engine._render.renderText(i_to_str(visualized->getCharacter()->getFire()).c_str(), engine._font, 0 + 25 + 20, 100);
		} else if(visualized->getClass() == ACTOR_BOMB){

			//render fire
			engine._render.renderSprite(_fire, -15, -15);
			engine._render.renderText(i_to_str(visualized->getBomb()->getFire()).c_str(), engine._font, 0 + 25 + 20, 0);

			//render turn
			engine._render.renderSprite(_turn, -15, 50-15);
			engine._render.renderText(i_to_str(visualized->getBomb()->getTurn()).c_str(), engine._font, 0 + 25 + 20, 50);

		}

	}


	//render player
	if (_player == 0){
		engine._render.renderSprite(IMG_GFX::win1, 0 + 20 + 200, 0);
	}else if (_player == 1){
		engine._render.renderSprite(IMG_GFX::win2, 0 + 20 + 200, 0);
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
	bool sync = false;
	if(!_actionMsg.empty())
		sync = true;
	while(!_actionMsg.empty()){
		const char* msg = _actionMsg.front().c_str();
		Team* curteam = &stage.getTeam(_player);

		if(msg[1] == WRP_END_TURN){
			_field = true;

		}else if(msg[1] == WRP_MOVE){
			int tid = 0;
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeMove(msg,tid,pid,xx,yy);

			Character* act = &(stage.getTeam(tid).getCharacter(pid));

			Actors* tgt = stage.getActorAt(xx,yy);
			if(tgt != NULL){
				if(tgt->getClass() == ACTOR_POWUP) act->AddPowerUp(tgt->getPowerUp());
				else cout << "something on the way: " << tgt->getClass() << endl;
			}
			_dummie.init(tid);
			_movie.waitForMotion(stage._tileMap._map, act->getX(), act->getY(), xx, yy, &_dummie,act);

			stage.moveActor(act->getX(), act->getY(), xx, yy);//TODO validate movment

			curteam->actions[pid] = CHAR_MOVED;
			curteam->_state[pid].addCooldown(ACTIONMOVE,1);

			//move message
			if(menu._net == 1){
				engine._com.send((void*)msg,msg[0]);
			}

		}else if(msg[1] == WRP_DYNAMITE){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeDynamite(msg,pid,xx,yy);

			Character* act = &(curteam->getCharacter(pid));

			int index = insert(stage._bombs);
			bomb* b = stage._bombs[index];


			b->init(act->getFire(),_player,pid);

			if(act->queryPowerUp(POWUP_DETONATOR)) b->setTurn(-1);

			act->AddDynamite(index);
			stage.instantiateActor(b, xx, yy);

			curteam->actions[pid] = CHAR_END;

			engine._render.playSound("bomb");

		}else if(msg[1] == WRP_SHOT){
			int pid = 0;
			int dir = 0;

			WrpDecodeShot(msg,pid,dir);

			Character* act = &(curteam->getCharacter(pid));

			if(dir == WRP_RIGHT){
				act->_dir = GUN_E;
				act->setState(ANIM_READY_R);
			}else if(dir == WRP_LEFT){
				act->_dir = GUN_W;
				act->setState(ANIM_READY_L);
			}else if(dir == WRP_DOWN){
				act->_dir = GUN_S;
				act->setState(ANIM_READY_D);
			}else if(dir == WRP_UP){
				act->_dir = GUN_N;
				act->setState(ANIM_READY_U);
			}

			curteam->_state[pid].addCooldown(ACTIONGUNFIRE,3);
			curteam->actions[pid] = CHAR_END;

			engine._render.playSound("shot");
		}else if(msg[1] == WRP_TIME_UP){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeTimeUp(msg,pid,xx,yy);

			bomb* b = stage.getActorAt(xx, yy)->getBomb();
			if(b != NULL) b->addTurn(1);

			curteam->actions[pid] = CHAR_END;

			engine._render.playSound("time_up");
		}else if(msg[1] == WRP_TIME_DOWN){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeTimeDown(msg,pid,xx,yy);

			bomb* b = stage.getActorAt(xx, yy)->getBomb();
			if(b != NULL) b->addTurn(-1);

			curteam->actions[pid] = CHAR_END;

			engine._render.playSound("time_down");
		}else if(msg[1] == WRP_THROW){
			int pid = 0;
			int xi = 0;
			int yi = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeThrow(msg,pid,xi,yi,xx,yy);

			bomb* b = stage.getActorAt(xi, yi)->getBomb();
			if(b != NULL){
				b->setPos(xx, yy);
				stage.getTileMap().at(xx, yy).actor = b;
				stage.getTileMap().at(xi, yi).actor = NULL;
			}

			curteam->actions[pid] = CHAR_END;

		}else if(msg[1] == WRP_BARREL){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeBarrel(msg,pid,xx,yy);

			int index = insert(stage._blocks);
			block* b = stage._blocks[index];
			b->setAnimation(BLK_TABLE,BLK_ID_BARREL);
			b->setData(BLK_ID_BARREL);
			stage.instantiateActor(b, xx, yy);

			curteam->actions[pid] = CHAR_END;

			engine._render.playSound("barrel");
		}else if(msg[1] == WRP_ROPE){
			int pid = 0;
			int xx = 0;
			int yy = 0;

			WrpDecodeRope(msg,pid,xx,yy);

			Character* tgt = stage.getActorAt(xx,yy)->getCharacter();

			if(tgt != NULL){
				if(tgt->getClass() == ACTOR_CHAR){
					Team* opo = &stage.getTeam((_player+1)%2);
					opo->_state[opo->checkSelected(tgt)].addCooldown(ACTIONMOVE,2);
				}
			}
			curteam->_state[pid].addCooldown(ACTIONSTUN,2);
			curteam->actions[pid] = CHAR_END;

			engine._render.playSound("rope");
		}else if(msg[1] == WRP_DETONATE){
			int pid = 0;

			WrpDecodeDetonate(msg,pid);

			Character* act = &(curteam->getCharacter(pid));

			int exp[MAX_BOMBS];
			act->detonate(exp);

			for(int i=0;i<MAX_BOMBS;++i)
				if(exp[i] != -1)
					stage._bombs[exp[i]]->setTurn(1);
			
			curteam->actions[pid] = CHAR_END;
		}
		_actionMsg.pop();
	}

	if(menu._net == 1 && sync){
		char buf[4096] = "\0";
		int len;
		stage.encode(buf,len);
		engine._com.send(buf,len);
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

void Game::turnPlayer(int player){
	if(engine._input.isPress()){
		int val = 0;
		if(button_end_turn.checkCollision(engine._input.getX(), engine._input.getY())){
			val = 1;
		}
		int xm,ym;
		winToMat(engine._input.getX(), engine._input.getY(), stage.getCamera().getX(), stage.getCamera().getY(), stage.getCamera().getScale(), xm, ym);
		cout << "clicked at: " << xm << "," << ym << endl;
		_ui.update(xm,ym,player,val);
	}
}
void Game::turnField(){
	bool exp = false;
	bool gun = false;
	vector<Actors*> hits;
	//calc bomb to explode
	for(int i=0;i<stage._bombs.size();++i){
		bomb* b = stage._bombs[i];
		if(b->isActive())
		if(b->getTurn() != -1){
			b->addTurn(-1);
			if(b->getTurn() < 1){
				hits.push_back(b);
				//engine._render.playSound("explosion");
				exp = true;
			}
		}
	}

	//calc gunfire
	{
	Team* tbuf = &stage.getTeam(_player);
	for(int j=0;j<5;++j){
		if(tbuf->_state[j].getState(ACTIONGUNFIRE) != 2)
			continue;
		if(!stage.getTeam(_player).getCharacter(j).getAlive())
			continue;
		stage.checkGunfire(&tbuf->getCharacter(j),hits);
		//tbuf->_state[j].setState(ACTIONGUNFIRE,STATE_STAGE2);
		//engine._render.playSound("gunfire");
		gun = true;

		if(tbuf->getCharacter(j)._dir == GUN_N){
			tbuf->getCharacter(j).setState(ANIM_SHOT_U);
		}else if(tbuf->getCharacter(j)._dir == GUN_S){
			tbuf->getCharacter(j).setState(ANIM_SHOT_D);
		}else if(tbuf->getCharacter(j)._dir == GUN_W){
			tbuf->getCharacter(j).setState(ANIM_SHOT_L);
		}else if(tbuf->getCharacter(j)._dir == GUN_E){
			tbuf->getCharacter(j).setState(ANIM_SHOT_R);
		}
	}
	}
	//calc chain explosion
	for(int i=0;i<hits.size();++i){
		if(hits[i]->getClass() == ACTOR_BOMB)
			stage.checkExplosion(hits[i]->getBomb(),hits);
	}
	//apply damage
	while(!hits.empty()){
		if(hits.back()->getClass() == ACTOR_BOMB){
			bomb* b = hits.back()->getBomb();

			if(b->getOwner() != -1) stage.getTeam(b->getTeam()).getCharacter(b->getOwner()).RemoveEntry(b->getIndex());

			stage.getTileMap().at(hits.back()->getX(),hits.back()->getY()).actor = NULL;

			remove(stage._bombs,b->getIndex());
		}else if(hits.back()->getClass() == ACTOR_CHAR){
			stage.hitChar(hits.back()->getCharacter());
		}else if(hits.back()->getClass() == ACTOR_POWUP){
			stage.hitPowUp(hits.back()->getPowerUp());
		}else if(hits.back()->getClass() == ACTOR_BLOCK){
			stage.hitBlock(hits.back()->getBlock());
		}
		hits.pop_back();
	}
	//after gun cooldown
	for(int i=0;i<5;++i){
		if(stage.getTeam(_player).getCharacter(i).getAlive()){
			if(stage.getTeam(_player)._state[i].getState(ACTIONGUNFIRE) == 1){
				if(stage.getTeam(_player).getCharacter(i)._dir == GUN_E){
					stage.getTeam(_player).getCharacter(i).setState(ANIM_IDLE_R);
				}else if(stage.getTeam(_player).getCharacter(i)._dir == GUN_W){
					stage.getTeam(_player).getCharacter(i).setState(ANIM_IDLE_L);
				}else if(stage.getTeam(_player).getCharacter(i)._dir == GUN_N){
					stage.getTeam(_player).getCharacter(i).setState(ANIM_IDLE_U);
				}else if(stage.getTeam(_player).getCharacter(i)._dir == GUN_S){
					stage.getTeam(_player).getCharacter(i).setState(ANIM_IDLE_D);
				}
			}
		}
	}

	//checkEnd();
	if(stage.suddenDeath()){
		stage.dropBomb(stage.suddenDeath());
	}
	stage.addTurn();

	_field = false;
	_player = (_player+1)%2;

	
	stage.getTeam(_player).beginTurn();
	centerTeam(_player);

	if(menu._net == 1){
		char buf[4096] = "\0";
		int len;
		stage.encode(buf,len);
		engine._com.send(buf,len);
	}
	if(exp)
		engine._render.playSound("explosion");
	if(gun)
		engine._render.playSound("gunfire");

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

