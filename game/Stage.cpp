#include "Stage.h"
#include <time.h>
//#include <random>
#include "enum.h"
#include "Props.h"

#define SUDDENDEATH 20

Stage::Stage(){
	_turn = 0;
	_suddenDeath = SUDDENDEATH;
	_camera.setPos(400, 200);
	_camera.setScale(120);
	_font = NULL;

	//_action.setSprite("NULL");//HANDLE
}


Stage::~Stage(){
}


void Stage::init(font* f){
	_font = f;
}
/*void Stage::setGUI(int width, int height){

}*/

//TODO fix redundance
int Stage::loadStage(const char* path, resources& res){
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

		stage >> id;
		stage >> botton;
		stage >> top;
		stage >> collision;

		const char* path = "resources/image/";
		string filepath;
		if(strcmp(botton,"NULL")!=0){
			filepath = path;filepath += botton;
			paths[id].bot = res.loadImage(filepath.c_str());
		}else{
			paths[id].bot = 1;
		}
		if(strcmp(top,"NULL")!=0){
			filepath = path;filepath += top;
			paths[id].up = res.loadImage(filepath.c_str());
		}else{
			paths[id].up = 1;
		}
		paths[id].collision = collision;
	}

	//list<string> names;
	//
	////percorre todos os caminhos das imagens adicionando em uma lista
	//for (map<int, Tile>::iterator it = paths.begin(); it != paths.end(); it++){
	//	names.push_back(it->second.bot);
	//	names.push_back(it->second.up);
	//}
	//
	////remove todos os repetidos
	//names.sort();
	//names.unique();
	//
	////carrega todas as imagens da lista (pela referencia de rousources)
	//for (list<string>::iterator it = names.begin(); it != names.end(); it++){
	//
	//	if (strcmp(it->c_str(), "NULL") != 0){
	//		string s;
	//		s.assign("resources/image/");
	//		s.append(it->c_str());
	//		resources.loadImage(s.c_str(), it->c_str());
	//	}
	//}
	//
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

			_tileMap._map.at(_initPos[i][j][0],_initPos[i][j][1]).actor = (Actors*)(-1);
		}
	}

	//stage.close();

	//polulate();

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
			_tileMap._map.at(_initPos[i][j][0], _initPos[i][j][1]).setActor(&(_teams[i]->getCharacter(j)));
		}
	}
	
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

void Stage::renderMap(renderer& renderer){
	renderer.renderMap(_tileMap, _camera, false);
}

void Stage::renderActors(renderer& renderer){
	int w = getTileMap().width();
	int h = getTileMap().height();

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
				if(ptr->getBomb()->getTurn() >= 0)
					renderer.renderTextInMapCenter(i_to_str(ptr->getBomb()->getTurn()).c_str(),_camera,*_font,ptr->getX(),ptr->getY());
			}
		}
	}

	/*for(list<bomb>::iterator it = bombs.begin();it != bombs.end();++it){
		renderer.renderActorInMapCenter(*it,_camera);
	}
	for(list<block>::iterator it = blocks.begin();it != blocks.end();++it){
		renderer.renderActorInMapCenter(*it,_camera);
	}
	for(list<PowerUp>::iterator it = power_ups.begin();it != power_ups.end();++it){
		renderer.renderActorInMapCenter(*it,_camera);
	}
	for(int i=0;i<5;++i){
		renderer.renderActorInMapCenter(_teams[0]->getCharacter(i),_camera);
		renderer.renderActorInMapCenter(_teams[1]->getCharacter(i),_camera);
	}*/

	for(int i=0;i<_vfx.size();++i){
		if(_vfx[i].getAnimation().getState() == ANIM_IDLE_R){
			renderer.renderActorInMapCenter(_vfx[i],_camera,15,-15);
		}else if(_vfx[i].getAnimation().getState() == ANIM_IDLE_D){
			renderer.renderActorInMapCenter(_vfx[i],_camera,-15,-15);
		}if(_vfx[i].getAnimation().getState() == ANIM_IDLE_L){
			renderer.renderActorInMapCenter(_vfx[i],_camera,-15,-15);
		}if(_vfx[i].getAnimation().getState() == ANIM_IDLE_U){
			renderer.renderActorInMapCenter(_vfx[i],_camera,15,-15);
		}else{
			renderer.renderActorInMapCenter(_vfx[i],_camera);
		}
		
	}
	if(!_vfx.empty()){
		if(_vfx.back().getAnimation().isDone()){
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


	_vfx.clear();

	while(!_bombs.empty()){
		delete _bombs.back();
		_bombs.pop_back();
	}
	while(!_blocks.empty()){
		delete _blocks.back();
		_blocks.pop_back();
	}
	while(!_powerUps.empty()){
		delete _powerUps.back();
		_powerUps.pop_back();
	}
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

		int index = insert(_bombs);
		bomb* b = _bombs[index];
		b->init(2,index);
		b->setTurn(2);
		instantiateActor(b,xx,yy);
		++n;
		if(n>=num)
			break;

	}
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

		int index = insert(_blocks);
		block* b = _blocks[index];

		b->setAnimation(BLK_TABLE,BLK_ID_BOX);
		instantiateActor(b,pos[r]%w,pos[r]/w);
		cout << "block at: " << pos[r]%w << "," << pos[r]/w << endl;
	}

}
void Stage::spawn(int x, int y){
	vector<int> powup;
	for(int i=POWUP_NONE+1;i<POWUP_END;++i)
		if(i != POWUP_ACTIVE)
			powup.push_back(i);

	int index = insert(_powerUps);
	PowerUp* p = _powerUps[index];
	p->init(powup[rand()%powup.size()]);

	instantiateActor(p,x,y);
}

Actors* Stage::getActorAt(int x, int y){
	if (x<0 || y<0 || x >= _tileMap._map.width() || y >= _tileMap._map.height())
		return NULL;
	return _tileMap._map.at(x, y).actor;
}
Actors* Stage::getOverlayAt(int x, int y){
	/*for(int i=0;i<_overlay.size();++i)
		if(_overlay[i].getX() == x && _overlay[i].getY() == y)
			return &_overlay[i];
	return NULL;*/
	return NULL;
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
					_vfx.back().setAnimation(VFX_TABLE,VFX_ID_EXPLOSION);

				}/*else if(strcmp(_vfx[ap].getAnimation().getState(),"exp")!=0){
					//fx.setSprite("explosao.png");
					_vfx.push_back(fx);
					_vfx.back().getAnimation().loadAnim("resources/VFX_exp.txt");
					_vfx.back().getAnimation().setState("exp");
				}*///TODO handle this case
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

	if(c->_dir == GUN_N)
		j=-1;
	else if(c->_dir == GUN_S)
		j=1;
	else if(c->_dir == GUN_W)
		i=-1;
	else if(c->_dir == GUN_E)
		i=1;

	{
	Actors fx;
	fx.setPos(c->getX(), c->getY());
	
	_vfx.push_back(fx);
	if(c->_dir == GUN_E){
		_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFLARE,ANIM_IDLE_R);
	}else if(c->_dir == GUN_W){
		_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFLARE,ANIM_IDLE_L);
	}else if(c->_dir == GUN_N){
		_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFLARE,ANIM_IDLE_U);
	}else if(c->_dir == GUN_S){
		_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFLARE,ANIM_IDLE_D);
	}
	}
	for(int k=1;;++k){
		int xx = i*k+c->getX();
		int yy = j*k+c->getY();

		if(_tileMap.getGround(xx,yy) != 1)
			break;

		Actors fx;
		fx.setPos(xx, yy);
		int ap = findActorsPos(fx,_vfx);
		if(ap==-1){
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

			_vfx.push_back(fx);
			if(c->_dir == GUN_E){
				_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFIRE,ANIM_IDLE_R);
			}else if(c->_dir == GUN_W){
				_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFIRE,ANIM_IDLE_L);
			}else if(c->_dir == GUN_N){
				_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFIRE,ANIM_IDLE_U);
			}else if(c->_dir == GUN_S){
				_vfx.back().setAnimation(VFX_TABLE,VFX_ID_GUNFIRE,ANIM_IDLE_D);
			}
			
		}/*else if(strcmp(_vfx[ap].getAnimation().getState(),"gunfire")!=0){
			//fx.setSprite("explosao.png");
			_vfx.push_back(fx);
			if(c->direction == GUN_E){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_right.txt");
			}else if(c->direction == GUN_W){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_left.txt");
			}else if(c->direction == GUN_N){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_up.txt");
			}else if(c->direction == GUN_S){
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_down.txt");
			}else{
				_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire_right.txt");
			}
			//_vfx.back().getAnimation().loadAnim("resources/VFX_gunfire.txt");
			_vfx.back().getAnimation().setState("gunfire");
		}*/

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
	Actors* dst = _tileMap._map.at(x, y).actor;

	if(dst != NULL)
	if(dst->getClass() == ACTOR_POWUP)
		_powerUps[dst->getIndex()]->deactivate();

	_tileMap._map.at(x0, y0).actor->setPos(x, y);
	_tileMap._map.at(x, y).actor = _tileMap._map.at(x0, y0).actor;
	_tileMap._map.at(x0, y0).actor = NULL;

	if(_tileMap._map.at(x, y).actor->getClass() == ACTOR_CHAR){
		int dx, dy;
		dx = x-x0;
		dy = y-y0;
		if(dx*dx > dy*dy){
			if(dx > 0){
				_tileMap._map.at(x, y).actor->setState(ANIM_IDLE_R);
			}else{
				_tileMap._map.at(x, y).actor->setState(ANIM_IDLE_L);
			}
		}else{
			if(dy > 0){
				_tileMap._map.at(x, y).actor->setState(ANIM_IDLE_D);
			}else{
				_tileMap._map.at(x, y).actor->setState(ANIM_IDLE_U);
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
	remove(_powerUps,p->getIndex());
}
void Stage::hitBlock(block* b){
	if(b == NULL)
		return;

	_tileMap._map.at(b->getX(),b->getY()).setActor(NULL);
	spawn(b->getX(),b->getY());
	remove(_blocks,b->getIndex());
}


void Stage::encode(char* data, int& len){
	int p = -1;
	char* n;
	data[++p] = 0;//2byte total size
	data[++p] = 0;//2byte total size

	data[++p] = _turn & 0xff;
	data[++p] = _suddenDeath & 0xff;

	n = &(data[++p]);//num of bombs
	(*n) = 0;
	for(int i=0;i<_bombs.size();++i){
		if(_bombs[i]->isActive()){
			++(*n);
			int l=0;
			_bombs[i]->encode(&(data[++p]),l);
			p+=l-1;
		}
	}
	n = &(data[++p]);//num of locks
	(*n) = 0;
	for(int i=0;i<_blocks.size();++i){
		if(_blocks[i]->isActive()){
			++(*n);
			int l=0;
			_blocks[i]->encode(&(data[++p]),l);
			p+=l-1;
		}
	}
	n = &(data[++p]);//num of powerups
	(*n) = 0;
	for(int i=0;i<_powerUps.size();++i){
		if(_powerUps[i]->isActive()){
			++(*n);
			int l=0;
			_powerUps[i]->encode(&(data[++p]),l);
			p+=l-1;
		}
	}

	int l=0;
	_tileMap.encode(&(data[++p]),l);
	p+=l-1;

	for(int i=0;i<2;++i){
		int l=0;
		_teams[i]->encode(&(data[++p]),l);
		p+=l-1;
	}

	data[++p] = '\0';

	short* ss=(short*)data;
	(*ss) = ++p;
	len = p;
}
void Stage::decode(char* data){
	int p = 1;
	int n;

	_turn = data[++p];
	_suddenDeath = data[++p];

	n = data[++p];
	for(int i=0;i<n;++i){
		int index = insert(_bombs);
		bomb* b = _bombs[index];
		int s = data[++p];
		b->decode(&(data[p]));
		p+=s-1;
	}
	n = data[++p];
	for(int i=0;i<n;++i){
		int index = insert(_blocks);
		block* b = _blocks[index];
		int s = data[++p];
		b->decode(&(data[p]));
		p+=s-1;
	}
	n = data[++p];
	for(int i=0;i<n;++i){
		int index = insert(_powerUps);
		PowerUp* b = _powerUps[index];
		int s = data[++p];
		b->decode(&(data[p]));
		p+=s-1;
	}

	int l;
	l = l = *((short*)&(data[++p]));
	_tileMap.decode(&(data[p]),this);
	p+=l-1;

	for(int i=0;i<2;++i){
		l = *((short*)&(data[++p]));
		_teams[i]->decode(&(data[p]),&_tileMap);

		p+=l-1;
	}
}

