#include "Stage.h"
#include <time.h>
//#include <random>
#include "enum.h"

#define SUDDENDEATH 20

Stage::Stage(){
	_turn = 0;
	_suddenDeath = SUDDENDEATH;
	_camera.setPos(400, 200);
	_camera.setScale(120);
	_font = NULL;

	_action.setSprite("NULL");
}


Stage::~Stage(){
}


void Stage::init(font* f){
	_font = f;
}
/*void Stage::setGUI(int width, int height){

}*/

int Stage::loadStage(const char* path, resources& resources)
{
	int width;
	int height;
	int num_ids;
	map<int, Tile> paths;
	_turn = 0;
	_suddenDeath = SUDDENDEATH;
	stringstream stage;
	//stage.open(path, ios::in);
	readRW(stage, path);

	stage >> width;
	stage >> height;
	stage >> num_ids;

	

	for (int i = 0; i < num_ids; i++){

		char line[512];
		char top[512];
		char botton[512];

		int id;
		int collision;

		//stage.getline(line,512);
					// int string string int
		//sscanf(line, "%d %s %s %d", &id, botton, top, &collision);

		stage >> id;
		stage >> botton;
		stage >> top;
		stage >> collision;

		/*
		stage >> id;

		Tile t;
		stage >> t.bot;
		stage >> t.top;
		stage >> t.hit;
		*/

		paths[id].bot.assign(botton);
		paths[id].up.assign(top);
		paths[id].collision = collision;
	}

	list<string> names;
	
	//percorre todos os caminhos das imagens adicionando em uma lista
	for (map<int, Tile>::iterator it = paths.begin(); it != paths.end(); it++){
		names.push_back(it->second.bot);
		names.push_back(it->second.up);
	}

	//remove todos os repetidos
	names.sort();
	names.unique();

	//carrega todas as imagens da lista (pela referencia de rousources)
	for (list<string>::iterator it = names.begin(); it != names.end(); it++){

		if (strcmp(it->c_str(), "NULL") != 0){
			string s;
			s.assign("resources/image/");
			s.append(it->c_str());
			resources.loadImage(s.c_str(), it->c_str());
		}
	}
	
	//setar id das imagens, no tile map

	_tileMap.init(width, height);

	int id;
	for (int y = 0; y < height; y++){	
		for (int x = 0; x < width; x++){
			stage >> id;
			_tileMap.setTile(paths[id], x, y);
		}
	}

	//carregar posicao dos personagens de cada time
	for (int i = 0; i < 2; i++){ // equipe
		for (int j = 0; j < 5; j++){ // personagem

			//caregar posicao 
			stage >> _initPos[i][j][0];
			stage >> _initPos[i][j][1];
		}
	}

	//stage.close();

	polulate();

	return 1;
}

void Stage::start(Team *teamA, Team *teamB)
{
	_teams[0] = teamA;
	_teams[1] = teamB;

	_teams[0]->count = 5;
	_teams[1]->count = 5;

	//setar posicao de cada character de cada team
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 5; j++){
			//setar team
			_teams[i]->getCharacter(j).setPos(_initPos[i][j][0], _initPos[i][j][1]);
			_teams[i]->getCharacter(j).init(i);
			//setar no tile map, os actors de cada team
			_tileMap._map.at(_initPos[i][j][0], _initPos[i][j][1]).setActor( dynamic_cast<Actors*>(&(_teams[i]->getCharacter(j))) );
		}
	}

	//TODO
	//instanciar randomicamente blocos, com power ups
	
//#define DEBUG_MAP
	
#ifdef DEBUG_MAP
	PowerUp p;
	
	p.init(POWUP_SPEED);
	p.setPos(3, 5);
	power_ups.push_back(p);
	_tileMap._map.at(3, 5).setActor(&power_ups.back() );
	
	p.init(POWUP_BOMB);
	p.setPos(3, 7);
	power_ups.push_back(p);
	_tileMap._map.at(3, 7).setActor(&power_ups.back());

	p.init(POWUP_FIRE);
	p.setPos(5, 5);
	power_ups.push_back(p);
	_tileMap._map.at(5, 5).setActor(&power_ups.back());

	p.init(POWUP_SHIELD);
	p.setPos(5, 7);
	power_ups.push_back(p);
	_tileMap._map.at(5, 7).setActor(&power_ups.back());

	p.init(POWUP_BOMB_TIMER);
	p.setPos(3, 3);
	power_ups.push_back(p);
	_tileMap._map.at(3, 3).setActor(&power_ups.back());

	p.init(POWUP_BOMB_RELAUNCH);
	p.setPos(3, 1);
	power_ups.push_back(p);
	_tileMap._map.at(3, 1).setActor(&power_ups.back());

	p.init(POWUP_BARRIER);
	p.setPos(5, 1);
	power_ups.push_back(p);
	_tileMap._map.at(5, 1).setActor(&power_ups.back());

	p.init(POWUP_STUN);
	p.setPos(5, 3);
	power_ups.push_back(p);
	_tileMap._map.at(5, 3).setActor(&power_ups.back());

	p.init(POWUP_DETONATOR);
	p.setPos(7, 1);
	power_ups.push_back(p);
	_tileMap._map.at(7, 1).setActor(&power_ups.back());
#endif

}

void Stage::render(renderer& renderer){
	int w = getTileMap().width();
	int h = getTileMap().height();
	
	renderer.renderMap(_tileMap, _camera, false);

	
	for(int i=0;i<_overlay.size();++i)
		renderer.renderActorInMapCenter(_overlay[i], _camera);//render actors in tile

	for(int j=0;j<h;++j){
		for(int i=0;i<w;++i){
			Actors* ptr = _tileMap._map.at(i,j).actor;
			if(ptr == NULL)
				continue;
			if(ptr->getClass() == ACTOR_BLOCK || ptr->getClass() == ACTOR_POWUP)
				renderer.renderActorInMapCenter(*ptr,_camera,0,-15);
			else if(ptr->getClass() == ACTOR_CHAR)
				renderer.renderActorInMapCenter(*ptr,_camera,0,-30);
			else
				renderer.renderActorInMapCenter(*ptr,_camera);

			if(ptr->getClass() == ACTOR_BOMB){
				if(((bomb*)ptr)->getTurn() >= 0)
					renderer.renderTextInMapCenter(i_to_str(((bomb*)ptr)->getTurn()).c_str(),_camera,*_font,ptr->getX(),ptr->getY());
			}
		}
	}
	renderer.renderActorInMapCenter(_action,_camera,-15,-75);

	for(int i=0;i<_vfx.size();++i){
		if(strcmp(_vfx[i].getAnimation().getState(),"gunfire_right")==0 || strcmp(_vfx[i].getAnimation().getState(),"gunflare_right")==0){
			renderer.renderActorInMapCenter(_vfx[i],_camera,15,-15);
		}else if(strcmp(_vfx[i].getAnimation().getState(),"gunfire_down")==0 || strcmp(_vfx[i].getAnimation().getState(),"gunflare_down")==0){
			renderer.renderActorInMapCenter(_vfx[i],_camera,-15,-15);
		}else if(strcmp(_vfx[i].getAnimation().getState(),"gunfire_left")==0 || strcmp(_vfx[i].getAnimation().getState(),"gunflare_left")==0){
			renderer.renderActorInMapCenter(_vfx[i],_camera,-15,-15);
		}else if(strcmp(_vfx[i].getAnimation().getState(),"gunfire_up")==0 || strcmp(_vfx[i].getAnimation().getState(),"gunflare_up")==0){
			renderer.renderActorInMapCenter(_vfx[i],_camera,15,-15);
		}else{
			renderer.renderActorInMapCenter(_vfx[i],_camera);
		}
	}
	if(!_vfx.empty()){
		if(_vfx.back().getAnimation().isDone(_vfx.back().getAnimation().getState())){
			_vfx.clear();
		}
	}
	
	renderer.renderMap(_tileMap, _camera, true);

}

void Stage::clear(){
	for( int x = 0; x < _tileMap._map.width(); x++){
		for(int y = 0; y < _tileMap._map.height(); y++){
			_tileMap._map.at(x, y).actor = NULL;
		}
	}

	bombs.clear();
	blocks.clear();
	power_ups.clear();
	_overlay.clear();
	_vfx.clear();

}


int Stage::suddenDeath(){
	int val = _turn-_suddenDeath;
	if(val<=0)
		return 0;
	return val;
}
void  Stage::addTurn(){
	++_turn;
}
void Stage::dropBomb(int num){
	int n=0;
	srand(time(NULL));
	for(int i=0;i<10*num;++i){
		int xx = (rand()%_tileMap._map.width()+_tileMap._map.width())%_tileMap._map.width();
		int yy = (rand()%_tileMap._map.height()+_tileMap._map.height())%_tileMap._map.height();

		if(_tileMap._map.at(xx,yy).collision != 1)
			continue;

		if(_tileMap._map.at(xx,yy).actor != NULL)
			continue;

		bomb b;
		b.init(2,NULL);
		b.setTurn(2);
		bombs.push_back(b);
		instantiateActor(&bombs.back(),xx,yy);
		++n;
		if(n>=num)
			break;

	}
}

void Stage::setAction(const char* name, int x, int y){
	_action.setPos(x,y);
	_action.setSprite(name);
}

matMN<Tile>& Stage::getTileMap(){
	return _tileMap._map;
}

camera& Stage::getCamera(){
	return _camera;
}


void Stage::polulate(){
	vector<int> pos;
	int w = getTileMap().width();
	int h = getTileMap().height();
	for(int j=0;j<h;++j){
		for(int i=0;i<w;++i){
			if(_tileMap._map.at(i,j).collision != 1)
				continue;
			if(_tileMap._map.at(i,j).actor != NULL)
				continue;

			pos.push_back(j*w+i);
		}
	}

	srand(time(NULL));

	for(int k=0;k<20;++k){
		int r = rand()%pos.size();

		if(_tileMap._map.at(pos[r]).actor != NULL)
			continue;

		block b;
		b.setSprite("BLK_box");

		blocks.push_back(b);
		instantiateActor(&blocks.back(),pos[r]%w,pos[r]/w);

	}


}
void Stage::spawn(int x, int y){
	vector<int> powup;
	for(int i=POWUP_NONE+1;i<POWUP_END;++i)
		if(i != POWUP_ACTIVE)
			powup.push_back(i);

	PowerUp p;
	p.init(powup[rand()%powup.size()]);
	power_ups.push_back(p);

	instantiateActor(&power_ups.back(),x,y);
}

Actors* Stage::getActorAt(int x, int y){
	if (x<0 || y<0 || x >= _tileMap._map.width() || y >= _tileMap._map.height())
		return NULL;
	return _tileMap._map.at(x, y).actor;
}
Actors* Stage::getOverlayAt(int x, int y){
	for(int i=0;i<_overlay.size();++i)
		if(_overlay[i].getX() == x && _overlay[i].getY() == y)
			return &_overlay[i];
	return NULL;
}
	
void Stage::clearOverlays(){
	_overlay.clear();
}

void Stage::markEntry(Actors* entry){
	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;
	Actors ol;
	ol.setPos(entry->getX(),entry->getY());
	ol.setSprite("OL_marker");
	_overlay.push_back(ol);
}
void Stage::markWalk(Character* entry){
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -entry->getSpeed();j<=entry->getSpeed();++j){
		for(int i = -entry->getSpeed();i<=entry->getSpeed();++i){

			if(abs(j)+abs(i) > entry->getSpeed())
				continue;

			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf != NULL){
				if(buf->getClass() != ACTOR_POWUP){
					continue;
				}
			}

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_walk");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markBomb(Character* entry){
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -2;j<=2;++j){
		for(int i = -2;i<=2;++i){

			if(abs(j)+abs(i) > 2)
				continue;

			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf != NULL)
				continue;
			

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_bomb");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markGunFire(Character* entry){
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -1;j<=1;++j){
		for(int i = -1;i<=1;++i){
			if(i==0&&j==0)
				continue;
			if(i!=0&&j!=0)
				continue;

			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(!_tileMap.existPos(xx,yy))
				continue;

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_gunfire");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markTimerUp(Character* entry){
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;
	int range = 4;
	for(int j = -range;j<=range;++j){
		for(int i = -range;i<=range;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;
			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;
			

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_uptimer");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markTimerDown(Character* entry){
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;
	int range = 4;
	for(int j = -range;j<=range;++j){
		for(int i = -range;i<=range;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;
			

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_downtimer");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markThrow(Character* entry){//TODO sprite
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -1;j<=1;++j){
		for(int i = -1;i<=1;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_throw");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markDetonator(Character* entry){
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = 0;j<getTileMap().height();++j){
		for(int i = 0;i<getTileMap().width();++i){
			int xx = i;
			int yy = j;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_BOMB)
				continue;
			if(static_cast<bomb*>(buf)->getOwner() != static_cast<Character*>(entry))
				continue;

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_bomb");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markBarrel(Character* entry){//TODO sprite
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -2;j<=2;++j){
		for(int i = -2;i<=2;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf != NULL)
				continue;
			

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_barrel");
			_overlay.push_back(ol);
		}
	}
}
void Stage::markRope(Character* entry){//TODO sprite
	_overlay.clear();

	if(!_tileMap.inBound(entry->getX(),entry->getY()))
		return;

	for(int j = -2;j<=2;++j){
		for(int i = -2;i<=2;++i){
			int xx = i+entry->getX();
			int yy = j+entry->getY();

			if(_tileMap.getGround(xx,yy) != 1)
				continue;

			Actors* buf = _tileMap.getActor(xx,yy);
			if(buf == NULL)
				continue;
			if(buf->getClass() != ACTOR_CHAR)
				continue;
			if(!areEnemies(static_cast<Character*>(entry),static_cast<Character*>(buf)))
				continue;

			Actors ol;
			ol.setPos(xx, yy);
			ol.setSprite("OL_rope");
			_overlay.push_back(ol);
		}
	}
}

int Stage::findActorsPtr(Actors* entry, vector<Actors*>& data){
	for(int i=0;i<data.size();++i)
		if(data[i] == entry)
			return i;
	return -1;
}
int Stage::findActorsPos(Actors& entry, vector<Actors>& data){
	for(int i=0;i<data.size();++i)
		if(data[i].getX() == entry.getX() && data[i].getY() == entry.getY())
			return i;
	return -1;
}

void Stage::checkExplosion(bomb* b, vector<Actors*>& out){
	for(int j=-1;j<=1;++j){
		for(int i=-1;i<=1;++i){
			if(i != 0 && j != 0)
				continue;
			if(i == 0 && j == 0)
				continue;
			for(int k=1;k<=b->getFire();++k){
				int xx = i*k+b->getX();
				int yy = j*k+b->getY();

				if(_tileMap.getGround(xx,yy) != 1)
					break;

				Actors fx;
				fx.setPos(xx, yy);
				int ap = findActorsPos(fx,_vfx);
				if(ap==-1){
					//fx.setSprite("explosao.png");
					_vfx.push_back(fx);
					_vfx.back().getAnimation().loadAnim("resources/VFX_exp.txt");
					_vfx.back().getAnimation().setState("exp");
				}else if(strcmp(_vfx[ap].getAnimation().getState(),"exp")!=0){
					//fx.setSprite("explosao.png");
					_vfx.push_back(fx);
					_vfx.back().getAnimation().loadAnim("resources/VFX_exp.txt");
					_vfx.back().getAnimation().setState("exp");
				}
				Actors* buf = _tileMap.getActor(xx,yy);
				if(buf != NULL){
					if(findActorsPtr(buf,out) == -1)
						out.push_back(buf);
					if(buf->getClass() == ACTOR_BLOCK)
						break;
				}
			}
		}
	}
}
void Stage::checkGunfire(Character* c, vector<Actors*>& out){
	int i=0;
	int j=0;

	if(c->direction == GUN_N)
		j=-1;
	else if(c->direction == GUN_S)
		j=1;
	else if(c->direction == GUN_W)
		i=-1;
	else if(c->direction == GUN_E)
		i=1;

	{
	Actors fx;
	fx.setPos(c->getX(), c->getY());
	
	_vfx.push_back(fx);
	if(c->direction == GUN_E){
		_vfx.back().getAnimation().loadAnim("resources/VFX_gunflare_right.txt");
		_vfx.back().getAnimation().setState("gunflare_right");
	}else if(c->direction == GUN_W){
		_vfx.back().getAnimation().loadAnim("resources/VFX_gunflare_left.txt");
		_vfx.back().getAnimation().setState("gunflare_left");
	}else if(c->direction == GUN_N){
		_vfx.back().getAnimation().loadAnim("resources/VFX_gunflare_up.txt");
		_vfx.back().getAnimation().setState("gunflare_up");
	}else if(c->direction == GUN_S){
		_vfx.back().getAnimation().loadAnim("resources/VFX_gunflare_down.txt");
		_vfx.back().getAnimation().setState("gunflare_down");
	}
	}
	for(int k=1;;++k){
		int xx = i*k+c->getX();
		int yy = j*k+c->getY();

		if(_tileMap.getGround(xx,yy) != 1)
			break;

		Actors fx;
		fx.setPos(xx, yy);
		//int ap = findActorsPos(fx,_vfx);
		//if(ap==-1){
			/*
			VFX_gunfire_down.txt
			VFX_gunfire_left.txt
			VFX_gunfire_right.txt
			VFX_gunfire_up.txt
			VFX_gunflare_down.txt
			VFX_gunflare_left.txt
			VFX_gunflare_right.txt
			VFX_gunflare_up.txt
			*/
			//fx.setSprite("explosao.png");
			_vfx.push_back(fx);
			if(c->direction == GUN_E){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_right.txt");
				_vfx.back().getAnimation().setState("gunfire_right");
			}else if(c->direction == GUN_W){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_left.txt");
				_vfx.back().getAnimation().setState("gunfire_left");
			}else if(c->direction == GUN_N){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_up.txt");
				_vfx.back().getAnimation().setState("gunfire_up");
			}else if(c->direction == GUN_S){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_down.txt");
				_vfx.back().getAnimation().setState("gunfire_down");
			}
			//_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire.txt");
			//_vfx.back().getAnimation().setState("gunfire");
		//}else if(strcmp(_vfx[ap].getAnimation().getState(),"gunfire")!=0){
		//	//fx.setSprite("explosao.png");
		//	_vfx.push_back(fx);
		//	if(c->direction == GUN_E){
		//		_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_right.txt");
		//	}else if(c->direction == GUN_W){
		//		_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_left.txt");
		//	}else if(c->direction == GUN_N){
		//		_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_up.txt");
		//	}else if(c->direction == GUN_S){
		//		_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_down.txt");
		//	}else{
		//		_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_right.txt");
		//	}
		//	//_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire.txt");
		//	_vfx.back().getAnimation().setState("gunfire");
		//}

		Actors* buf = _tileMap.getActor(xx,yy);
		if(buf != NULL){
			if(findActorsPtr(buf,out) == -1)
				if(buf->getClass() != ACTOR_BOMB)
					out.push_back(buf);
			if(buf->getClass() == ACTOR_BLOCK)
				break;
		}
	}

}

bool Stage::waitForVFX(){
	return !_vfx.empty();
}

void Stage::moveActor(int x0, int y0, int x, int y){
	
	if (_tileMap._map.at(x, y).actor != NULL){
		if (_tileMap._map.at(x, y).actor->getClass() == ACTOR_BOMB){
			//nao move por cima de bomba
			return;
		}
		else if (_tileMap._map.at(x, y).actor->getClass() == ACTOR_BLOCK){
			//nao move por cima de bloco
			return;
		}
		else if (_tileMap._map.at(x, y).actor->getClass() == ACTOR_CHAR){
			//nao move por cima de char
			return;
		}
		else if (_tileMap._map.at(x, y).actor->getClass() == ACTOR_POWUP){
			//move por cima de power up

			//remove power up da lista de renderizados
			for (list<PowerUp>::iterator it = power_ups.begin(); it != power_ups.end(); it++){
				if (it->getX() == _tileMap._map.at(x, y).actor->getX() && it->getY() == _tileMap._map.at(x, y).actor->getY()){
					power_ups.erase(it);
					break;
				}
			}

			_tileMap._map.at(x0, y0).actor->setPos(x, y);

			_tileMap._map.at(x, y).actor = _tileMap._map.at(x0, y0).actor;

			_tileMap._map.at(x0, y0).actor = NULL;
			

		}
		
	}
	else { // actor == null 
		//move por cima de tile com actor null

		//nova posicao
		_tileMap._map.at(x0, y0).actor->setPos(x, y);

		//
		_tileMap._map.at(x, y).actor = _tileMap._map.at(x0, y0).actor;

		_tileMap._map.at(x0, y0).actor = NULL;
	}

	if(_tileMap._map.at(x, y).actor->getClass() == ACTOR_CHAR){
		int dx, dy;
		dx = x-x0;
		dy = y-y0;
		if(dx*dx > dy*dy){
			if(dx > 0){
				_tileMap._map.at(x, y).actor->setState("idle_right");
			}else{
				_tileMap._map.at(x, y).actor->setState("idle_left");
			}
		}else{
			if(dy > 0){
				_tileMap._map.at(x, y).actor->setState("idle_down");
			}else{
				_tileMap._map.at(x, y).actor->setState("idle_up");
			}
		}
	}

}

void Stage::instantiateActor(Actors* a, int x, int y){
	a->setPos(x, y);
	_tileMap._map.at(x, y).actor = a;
}

Team& Stage::getTeam(int player){
	return *_teams[player];
}
bool Stage::areEnemies(Character* c1, Character* c2){
	if(_teams[0]->isMember(c1) && _teams[0]->isMember(c2))
		return false;
	if(_teams[1]->isMember(c1) && _teams[1]->isMember(c2))
		return false;
	return true;
}

void Stage::hitChar(Character* c){
	if(c == NULL)
		return;

	if(c->getShield() > 0){
		c->AddShield(-1);
		return;
	}
	c->setAlive(false);

	_tileMap._map.at(c->getX(),c->getY()).setActor(NULL);

	if(_teams[0]->isMember(c))
		_teams[0]->count -= 1;
	else
		_teams[1]->count -= 1;

}
void Stage::hitPowUp(PowerUp* p){
	if(p == NULL)
		return;

	_tileMap._map.at(p->getX(),p->getY()).setActor(NULL);

	for(list<PowerUp>::iterator it=power_ups.begin();it!=power_ups.end();it++){
		if(&*it == p){
			power_ups.erase(it);
			break;
		}
	}
}
void Stage::hitBlock(block* b){
	if(b == NULL)
		return;

	_tileMap._map.at(b->getX(),b->getY()).setActor(NULL);
	spawn(b->getX(),b->getY());
	for(list<block>::iterator it=blocks.begin();it!=blocks.end();it++){
		if(&*it == b){
			blocks.erase(it);
			break;
		}
	}
}