#include "motion.h"
#include "translator.h"
#include "..\game\Props.h"

void dirToCoord(int dir,int* vec){
	switch(dir){
	case DIR_N:{
		--vec[1];
		break;
				}
	case DIR_S:{
		++vec[1];
		break;
				}
	case DIR_E:{
		++vec[0];
		break;
				}
	case DIR_W:{
		--vec[0];
		break;
				}
	}
}

void killVec(vector<a_node*>& vec){
	for(int i=0;i<vec.size();++i)
		delete vec[i];
	vec.clear();
}

int findNode(vector<a_node*>& vec, a_node* data){
	for(int i=0;i<vec.size();++i)
		if(vec[i]->_x == data->_x && vec[i]->_y == data->_y)
			return i;
	return -1;
}
int findNode(vector<a_node*>& vec, int x, int y){
	for(int i=0;i<vec.size();++i)
		if(vec[i]->_x == x && vec[i]->_y == y)
			return i;
	return -1;
}
int minNode(vector<a_node*>& vec){
	float val=5000.0f;
	int num = -1;

	for(int i=0;i<vec.size();++i){
		if(vec[i]->_f < val){
			val = vec[i]->_f;
			num = i;
		}
	}
	return num;
}

float distance(float x1, float y1, float x2, float y2){
	//return sqrtf(powf(x1-x2,2.0f)+powf(y1-y2,2.0f));
	return sqrtf(powf(x1-x2,2.0f))+sqrtf(powf(y1-y2,2.0f));
}

void getPatch(matMN<Tile>& map, int x0, int y0, int x, int y, vector<int>& out){
	vector<a_node*> open;
	vector<a_node*> closed;

	a_node* buf;

	out.clear();

	buf = new a_node();
	buf->_parent = NULL;
	buf->_x = x0;
	buf->_y = y0;
	buf->_h = distance(x0,y0,x,y);
	buf->_g = 0.0f;
	buf->_f = 0.0f;

	open.push_back(buf);

	buf = NULL;

	while(!open.empty()){
		int cur;
		cur = minNode(open);

		//if equals target
		if(open[cur]->_x == x && open[cur]->_y == y){
			buf = open[cur];
			break;
		}
		closed.push_back(open[cur]);
		open.erase(open.begin()+cur);


		for(int j=1;j>-2;--j){
			for(int i=-1;i<2;++i){
				//if middle skip
				if(i==0 && j==0)
					continue;
				//if corner skip
				if(i!=0 && j!=0)
					continue;
				//calc new coord
				int xx = closed.back()->_x+i;
				int yy = closed.back()->_y+j;
				int gg;
				int same;
				//if out of bounds skip
				if(xx < 0 || xx >= map.width() || yy < 0 || yy >= map.height())
					continue;
				//if block if not walkable
				if(map.at(xx,yy).collision == 0)
					continue;
				//if(map.at(xx,yy).collision == 2 && map.at(closed.back()->_x,closed.back()->_y).collision != 4)
				//	continue;
				if(map.at(xx,yy).actor != NULL)
					continue;

				//calc walk cost G
				if(i == 0 || j == 0)
					gg = closed.back()->_g+10.0f;
				else
					gg = closed.back()->_g+14.0f;
				//search in the closed list
				same = findNode(closed,xx,yy);
				if(same != -1){
					if(closed[same]->_g <= gg)
						continue;

					closed[same]->_parent = closed.back();
					closed[same]->_g = gg;
					closed[same]->_f = open[same]->_g+open[same]->_h;
					continue;
				}
				//search in the open list
				same = findNode(open,xx,yy);
				if(same != -1){
					if(open[same]->_g <= gg)
						continue;

					open[same]->_parent = closed.back();
					open[same]->_g = gg;
					open[same]->_f = open[same]->_g+open[same]->_h;
					continue;
				}
				//add new node to open
				buf = new a_node();
				buf->_parent = closed.back();
				buf->_x = xx;
				buf->_y = yy;
				buf->_g = gg;
				buf->_h = distance(buf->_x,buf->_y,x,y);
				buf->_f = buf->_g+buf->_h;
				
				open.push_back(buf);
			}
		}
	}
	if(open.empty()){
		killVec(closed);
		return;
	}

	while(buf->_parent != NULL){
		if(buf->_y < buf->_parent->_y){			//north
			out.push_back(DIR_N);
		}else if(buf->_y > buf->_parent->_y){	//south
			out.push_back(DIR_S);
		}else if(buf->_x < buf->_parent->_x){	//west	
			out.push_back(DIR_W);
		}else if(buf->_x > buf->_parent->_x){	//east
			out.push_back(DIR_E);
		}else{
			out.push_back(-1);
		}
		buf = buf->_parent;
	}
	//reverse(out.begin(),out.end());

	killVec(open);
	killVec(closed);

	return;
}

camera* motion::_cam = NULL;

motion::motion(){
	_xm = 0;
	_ym = 0;

	_x0 = 0;
	_y0 = 0;
	_xv = 0;
	_yv = 0;
	_count = 0;

	//_cam = NULL;

	_moveDone = true;

	_anim = NULL;
	_animDone = true;
}
motion::~motion(){
}

void motion::init(camera* cam){
	_cam = cam;
}

void motion::waitForMotion(matMN<Tile>& map, int x0, int y0, int x, int y, Actors* act, Actors* real){
	if(_cam == NULL)
		return;
	_moveDone = false;
	_animDone = true;
	_xm = x0;
	_ym = y0;
	matToWin(x0,y0,_cam->getX(),_cam->getY(),_cam->getScale(),_x0,_y0);
	_xv = 0;
	_yv = 0;

	getPatch(map,x0,y0,x,y,_path);

	_count = WALKSPEED;

	_entity = act;
	_real = real;
	_real->setIndex(-2);
	_map = &map;

}
void motion::vaitForAnimation(animation* anim){
	if(_cam == NULL)
		return;
	_moveDone = true;
	_animDone = false;

	_anim = anim;
}

void motion::update(){
	if(!_moveDone){
		if(_count >= WALKSPEED){
			if(!_path.empty()){
				int dir = _path.back();
				_path.pop_back();

				int xw;
				int yw;
				_map->at(_xm,_ym).over = NULL;

				if(dir == DIR_N){
					_ym-=1;
					_entity->setState(ANIM_WALK_U);
				}else if(dir == DIR_S){
					_ym+=1;
					_entity->setState(ANIM_WALK_D);
				}else if(dir == DIR_E){
					_xm+=1;
					_entity->setState(ANIM_WALK_R);
				}else if(dir == DIR_W){
					_xm-=1;
					_entity->setState(ANIM_WALK_L);
				}
				_map->at(_xm,_ym).over = _entity;
				matToWin(_xm,_ym,_cam->getX(),_cam->getY(),_cam->getScale(),xw,yw);
				int xb = _x0 + _xv;
				int yb = _y0 + _yv;
				_xv = xw-xb;
				_yv = yw-yb;
				_x0 = xb;
				_y0 = yb;
				_count = 0;
			}else{
				_moveDone = true;
				_entity->setState(0);
				_entity->setState(1);
				_map->at(_xm,_ym).over = NULL;
				_entity = NULL;
				_real->setIndex(-1);
				_real = NULL;
				return;
			}
		}
		_count++;
		float dt =(float)(_count)/(float)(WALKSPEED);
		int xx = _x0+_xv*dt+3;
		int yy = _y0+_yv*dt-60;

		if(_entity->getAnimation().getState() == ANIM_IDLE_R || _entity->getAnimation().getState() == ANIM_IDLE_U){
			xx-=6;
		}

		_entity->setPos(xx,yy);

		//_animDone = true;
		//_moveDone = true;
	}
	if(!_animDone){
		_animDone = _moveDone;//true;//_anim->isDone();
	}
}
bool motion::isDone(){
	return _animDone & _moveDone;
}

