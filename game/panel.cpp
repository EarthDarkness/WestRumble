#include "panel.h"

panel::panel(){
}
panel::panel(int width, int height, float wSpacing, float hSpacing, guiEntry** members){
	init(width, height, wSpacing, hSpacing, members);
}
panel::~panel(){
	for(int i=0;i<_elements.width()*_elements.height();++i)
		if(_elements[i] != NULL)
			if(_elements[i]->doDestroy())
				delete _elements[i];
}

void panel::init(int width, int height, float wSpacing, float hSpacing, guiEntry** members){
	_wSpacing = wSpacing;
	_hSpacing = hSpacing;
	_elements.init(width, height);
	for(int i=0;i<width*height;++i)
		_elements.set(members[i], i);
}

void panel::setGUI(int x, int y, int w, int h){
	int wtsp = (float)w*_wSpacing;
	int htsp = (float)h*_hSpacing;

	int wrem = w-wtsp;
	int hrem = h-htsp;

	int wele = wrem/_elements.width();
	int hele = hrem/_elements.height();

	int wsp = wtsp/_elements.width();
	int hsp = htsp/_elements.height();


	for(int j=0;j<_elements.height();++j){
		for(int i=0;i<_elements.width();++i){
			int xx = x+i*(wsp+wele);
			int yy = y+j*(hsp+hele);

			if(_elements.at(i,j) == NULL)
				continue;

			_elements.at(i, j)->setGUI(xx, yy, wele, hele);
		}
	}
}

bool panel::doDestroy(){
	return true;
}
