#include <glad/glad.h>

#include "States.h"
#include "handler.h"
#include "utils.h"
#include "Saves.h"
#include "Text.h"

void drawButton(float x, float y, bool is) {
	glBegin(GL_QUADS);
	glColor3f(0.f, 1.f, 0.f);
	
	glVertex2f(x-0.5f, y-0.3f);
	glVertex2f(x+0.5f, y-0.3f);
	glVertex2f(x+0.5f, y+0.3f);
	glVertex2f(x-0.5f, y+0.3f);
	
	glEnd();
}

void ShopState::draw() {
	clear(0.f, 0.f, 0.f);
	drawCenteredString(doubleToString(saves::parts["money"]) + " monies", 0.f, 0.f, 1.5f);
}
void ShopState::update() {

}
void ShopState::onKeyPress(int key, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE) {
		exitToParent();
	}
}
void ShopState::onMouseClick(double x, double y, int action, int button) {

}