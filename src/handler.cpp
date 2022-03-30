#include <glad/glad.h>
#include <iostream>
#include <vector>

#include "utils.h"
#include "handler.h"
#include "Image.h"
#include "States.h"
#include "main.h"
#include "Text.h"

using namespace std;

bool loaded = false;
State* t;
State* goingTo;
State* sstack[16];
int currentPlace = 0;
bool transitioning = false;
float trans = 0.f;
bool goingUp = true;
bool isDebugMenu = true;
bool isPoppingBack = false;

void push_back(State* r) {
	if(++currentPlace == 16) {
		cerr << "STATE STACK OVERFLOW ERROR. Tried to add state to full stack. Exiting." << endl;
		exit(EXIT_FAILURE);
		return;
	}
	sstack[currentPlace] = r;
	t = sstack[currentPlace];
}

void pop_back() {
	delete sstack[currentPlace];
	currentPlace--;
	t = sstack[currentPlace];
}

State* back() {
	return sstack[currentPlace];
}

void startTransition() {
	t->onUnload();
	if(isPoppingBack) {
		pop_back();
	} else {
		push_back(goingTo);
	}
	t->onLoad();
}

void goToState(State* newState) {
    if(transitioning) {
        return;
    }
    goingTo = newState;
    startTransition();
}

void exitToParent() {
    if(transitioning) {
        return;
    }
    //pop_back();
	isPoppingBack = true;
    goingTo = back();
    startTransition();
}

void handOnLoad() {
	
}

void handlerUnfixedUpdate() {
	t->unfixedUpdate();
}

void handlerUpdate() {
    if(!loaded) {
        delete goingTo;
        delete t;
        t = new MenuState();
        push_back(t);
        loaded = true;
    }
    /*if(transitioning) {
        if(goingUp) {
            if(trans >= 1.f) {
				t->onUnload();
				if(isPoppingBack) {
					isPoppingBack = false;
					pop_back();
					trans = 1.f;
					goingUp = false;
					goingTo = NULL;
					t = back();
				} else {
					push_back(goingTo);
					trans = 1.f;
					goingUp = false;
					goingTo = NULL;
					t = back();
				}
				t->onLoad();
            } else {
                trans += 1.f/20.f;
            }
        } else {
            if(trans < 0.f) {
                transitioning = false;
                trans = 0.f;
            } else {
                trans -= 1.f/20.f;
            }
        }
    }*/
    t->update();
}

void onKey(int key, int action, int mods) {
	if(key == GLFW_KEY_D && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		isDebugMenu = !isDebugMenu;
	}
    t->onKeyPress(key, action, mods);
}

void onMouse(double x, double y, int action, int button) {
    t->onMouseClick(x, y, action, button);
}

void handlerDraw() {
    //Draw scene
    t->draw();

    resetMatrix();

    //draw transition fade
    /*if(transitioning) {
        glBegin(GL_QUADS);

        glColor4f(0.f, 0.f, 0.f, trans);

        glVertex2f(-xStretch, -yStretch);
        glVertex2f(-xStretch, yStretch);
        glVertex2f(xStretch, yStretch);
        glVertex2f(xStretch, -yStretch);

        glEnd();

        glColor4f(0.f, 0.f, 0.f, 1.f);
    }*/

    //Draw lines of scaled box to draw in
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_LINES);

    glColor4f(0.f, 0.f, 0.f, 1.f);

    glVertex2f(-(16.f/9.f), 1.f);
    glVertex2f((16.f/9.f), 1.f);

    glVertex2f(-(16.f/9.f), -1.f);
    glVertex2f((16.f/9.f), -1.f);

    glVertex2f((16.f/9.f), 1.f);
    glVertex2f((16.f/9.f), -1.f);

    glVertex2f(-(16.f/9.f), 1.f);
    glVertex2f(-(16.f/9.f), -1.f);

    glEnd();
	
	if(isDebugMenu) drawString(t->getDebugMenu(), -(16.f/9.f), 1.f, 1.f, 1.f, 1.f, 0.f);
	
	t->postProcessing();
}
