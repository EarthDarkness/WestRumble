#include "game\Game.h"

int main(int argc, char** argv) {

	Game gm;
	gm.init();

	//teste commit

	while (gm.engine.isRunning()) {
		gm.update();
		gm.render();
	}
	return 0;
}