#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "States.h"
#include "handler.h"
#include "utils.h"
#include "Text.h"
#include "main.h"
#include "Resources.h"
#include "Saves.h"

using namespace std;

void State::update() {}
void State::draw() {}
void State::onKeyPress(int key, int action, int mods) {}
void State::onMouseClick(double x, double y, int action, int button) {}
State::State() {}

/*
base state functions
void State::draw() {

}
void State::update() {

}
void State::onKeyPress(int key, int action, int mods) {

}
void State::onMouseClick(double x, double y, int action, int button) {

}
*/

void MenuState::draw() {
    clear(0.5f, 0.5f, 1.f);

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);

    for(int i = 0; i < 4; i++) {
        plotPoint(rotate(points[i], hrot, 0.f, -0.35f));
    }

    glEnd();

    glPopMatrix();

    drawCenteredString("Play Bang", 0.f, -0.35f, (rot/30)+1.f, 1.f, 0.f, 0.f);
}
void MenuState::update() {
    if(!(getX() > -1.f && getX() < 1.f && getY() > -0.5f && getY() < -0.2f)) {
        rot = 0;
        rotvel = 0;
        goingup = false;
        hrot = 0;
        return;
    }
    if(rot < 0) {
        goingup = !goingup;
        rotvel = 1.5f;
        rot = 0;
    }
    rot += rotvel;
    rotvel -= 0.07;
    if(goingup) {
        hrot = rot;
    } else {
        hrot = -rot;
    }
}
void MenuState::onKeyPress(int key, int action, int mods) {
	if(key == GLFW_KEY_L) {
		goToState(new TestState());
	}
}
void MenuState::onMouseClick(double x, double y, int action, int button) {
    //cout << "clicked" << endl;
    if(x > -1.f && x < 1.f && y > -0.5f && y < -0.2f) {
        goToState(new MainMenuState());
    }
}
double dis;

void MainMenuState::draw() {
	clear(1.f, 0.5f, 0.5f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	fillOval(0.f, -0.7f, 0.25f, 0.25f, 50);
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_TRIANGLES);
	
	glVertex2f(-0.11f, -0.57f);
	glVertex2f(-0.11f, -0.83f);
	glVertex2f(0.13f, -0.7f);
	
	glEnd();
	
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	
	glVertex2f(-1.6f, 0.9f);
	glVertex2f(-1.6f, 0.7f);
	glVertex2f(-1.f,  0.7f);
	glVertex2f(-1.f,  0.9f);
	
	glColor3f(1.f, 0.f, 0.f);
	
	float off = (saves::masterSaveParts["xp"]/saves::getXPforLevel((int)saves::masterSaveParts["xpLevel"])) * 0.52f;
	glVertex2f(-1.56f, 0.86f);
	glVertex2f(-1.56f, 0.74f);
	glVertex2f(-1.56f+off,  0.74f);
	glVertex2f(-1.56f+off,  0.86f);
	
	glEnd();
	
	drawCenteredString(to_string((int)saves::masterSaveParts["xpLevel"]), -1.7f, 0.8f, 2.f);
	drawCenteredString(doubleToString(saves::masterSaveParts["xp"]) + "/" + doubleToString(saves::getXPforLevel()), -1.3f, 0.8f, 1.f);
}
void MainMenuState::update() {
	dis = sqrt(getX()*getX()+(getY()+0.7d)*(getY()+0.7d));
}
void MainMenuState::onKeyPress(int key, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE) {
		bangExit();
	}
}
void MainMenuState::onMouseClick(double x, double y, int action, int button) {
	if(dis < 0.25d) {
		goToState(new GameChooserState());
	}
}

const GameSettings basicSettings = {30, 25.d, 0.1f, 1.f, 1.f, "basicSave.bin"};

void GameChooserState::draw() {
	clear(0.f, 0.f, 0.f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	fillOval(0.f, -0.7f, 0.25f, 0.25f, 50);
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_TRIANGLES);
	
	glVertex2f(-0.11f, -0.57f);
	glVertex2f(-0.11f, -0.83f);
	glVertex2f(0.13f, -0.7f);
	
	glEnd();
}
void GameChooserState::update() {
	dis = sqrt(getX()*getX()+(getY()+0.7d)*(getY()+0.7d));
}
void GameChooserState::onKeyPress(int key, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE) {
		exitToParent();
	}
}
void GameChooserState::onMouseClick(double x, double y, int action, int button) {
	if(dis < 0.25d) {
		GameSettings next = basicSettings;
		saves::parts["money"] = (double)next.money;
		saves::parts["hits"] = (double)next.hits;
		saves::parts["power"] = (double)next.power;
		saves::savePath = next.savePath;
		saves::read(saves::getSavePath(), next);
		goToState(new PlayState());
	}
}

void TestState::update() {

}

void TestState::draw() {
    glBegin(GL_QUADS);
    if(isGreen) {
        glColor3f(0.f, 1.f, 0.f);
    } else {
        glColor3f(1.f, 0.f, 0.f);
    }

    glVertex2f(-0.75f, -0.75f);
    glVertex2f(-0.75f, 0.75f);
    glVertex2f(0.75f, 0.75f);
    glVertex2f(0.75f, -0.75f);

    glEnd();

    glEnable(GL_TEXTURE_2D);
    res::container.bind();
    glBegin(GL_QUADS);

    glColor4f(1.f, 1.f, 1.f, 1.f);

    glVertex2f(-0.5f, -0.5f);
    glTexCoord2f(0.f, 1.f);

    glVertex2f(-0.5f, 0.5f);
    glTexCoord2f(0.f, 0.f);

    glVertex2f(0.5f, 0.5f);
    glTexCoord2f(1.f, 0.f);

    //glVertex2f(0.f, -1.f);
    glVertex2f(getX(), getY());
    glTexCoord2f(1.f, 1.f);

    glEnd();

    drawString("Current time: " + std::to_string(glfwGetTime()), getX(), getY(), 1.f, 1.f, 0.f, 0.f);
}

void TestState::onKeyPress(int key, int action, int mods) {
    if(key == GLFW_KEY_SPACE) {
        isGreen = !isGreen;
    }
}

void TestState::onMouseClick(double x, double y, int action, int button) {

}
