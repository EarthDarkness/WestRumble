#include "game\Game.h"

int main(int argc, char** argv) {

	Game gm;
	gm.init();

	while (gm.engine.isRunning()) {
		gm.update();
		gm.render();
	}

	return 0;
}