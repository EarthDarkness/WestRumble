#ifndef PANEL_H
#define PANEL_H

#include "Button.h"

#include <vector>


enum PANEL_STYLE{
	PANEL_VERTICAL=0,
	PANEL_HORIZONTAL,
	PANEL_MATRIX
};

class panel
{
public:
	int _type;//PANEL_STYLE
	vector<bool> _isPanel;
	vector<Button*> _buttons;
	vector<panel*> _panels;
};


#endif


