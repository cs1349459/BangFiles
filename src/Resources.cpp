#include "Resources.h"
#include "Image.h"

namespace res {
	Image container, ball, pointer, groundPath, stadium, hills, clouds, mountain;
	void load() {
		container = loadImage("images/container.jpg", false);
		ball      = loadImage("images/ball.png", true);
		pointer   = loadImage("images/pointer.png", true);
		groundPath= loadImage("images/ground.png", true);
		stadium   = loadImage("images/stadium.png", true);
		hills     = loadImage("images/hills.png", true);
		clouds    = loadImage("images/clouds.png", true);
		mountain  = loadImage("images/mountain.png", true);
	}
}
