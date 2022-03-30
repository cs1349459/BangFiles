#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "States.h"
#include "Resources.h"
#include "Text.h"
#include "Saves.h"
#include "handler.h"

const float ballSize = 0.03f;

float camX, camY;
float ballX, ballY;
float ballXView, ballYView;
float ballXVel, ballYVel;
int pointerPlace = 0;
float launchPower = -1.f;
bool isStopped = false;

int framesSinceBouncing = -1;
int framesStopped = 0;
int framesSinceStateChange = 0;

float ballAngle = 0.f;
float ballAngleVel = 3.f;

string state = "aiming";

int resetInt = 0;
int resetFrames = 0;
bool isPaused = false;
float speed = 1.f;

float getXChange() {
	return ballXVel/10.f;
}

float getYChange() {
	return ballYVel/10.f;
}

float xFrameChange() {
	return getXChange()*speed*8.f;
}

float yFrameChange() {
	return getYChange()*speed*8.f;
}

void reset() {
	ballX = 0.f;
	ballY = 0.f;
	launchPower = -1.f;
	isStopped = false;
	framesSinceBouncing = -1;
	framesStopped = 0;
	framesSinceStateChange = 0;
	ballAngle = 0.f;
	ballAngleVel = 3.f;
	speed = 1;
	resetInt = 0;
	isPaused = false;
	state = "aiming";
}

void changeState(string newState) {
	state = newState;
	framesSinceStateChange = 0;
}

void drawBar() {
	if(framesSinceStateChange >= 90 && state == "playing") return;
	if(framesSinceStateChange < 90 && state == "playing") {
		glPushMatrix();
		glTranslatef(((float)min(90-framesSinceStateChange, 60)/240.f)-0.25f, 0.f, 0.f);
	}
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex2f(-(16.f/9.f), 1.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(-(16.f/9.f), 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(-(16.f/9.f)+0.1f, 0.f);
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex2f(-(16.f/9.f)+0.1f, 1.f);
	
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex2f(-(16.f/9.f), -1.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(-(16.f/9.f), 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(-(16.f/9.f)+0.1f, 0.f);
	glColor3f(0.25f, 0.25f, 0.25f);
	glVertex2f(-(16.f/9.f)+0.1f, -1.f);
	
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.f, 1.f, 1.f);
	res::pointer.bind();
	drawTexturedQuad(-(16.f/9.f)+0.1f, (float)(pointerPlace/1000.f)-0.0375f, 0.15f, 0.075f);
	if(framesSinceStateChange < 90 && state == "playing") glPopMatrix();
}

void drawBackground() {
	glColor3f(1.f, 1.f, 1.f);
	res::mountain.bind();
	for(float i = -2.f; i < 4.f; i += 25.f) {
		drawTexturedQuad(i-fmod(camX/25000, 25.f), -1.f-camY/25000, 1.f, 1.f);
	}
	res::clouds.bind();
	for(float i = -2.f; i < 4.f; i += 2.f) {
		drawTexturedQuad(i-fmod(camX/2500, 2.f), -0.7f-camY/2500, 2.f, 0.6f);
		drawTexturedQuad(i-fmod(camX/2500, 2.f), -0.7f-camY/2500, 2.f, -0.6f);
	}
	res::hills.bind();
	for(float i = -2.f; i < 4.f; i += 2.f) {
		drawTexturedQuad(i-fmod(camX/250, 2.f), -1.f-camY/250, 2.f, 0.6f);
	}
	res::stadium.bind();
	for(float i = -2.f; i < 4.f; i += 1.f) {
		drawTexturedQuad(i-fmod(camX/5, 1.f), -0.8f-camY/5, 1.f, 0.4f);
	}
	res::groundPath.bind();
	for(float i = -2.f; i < 4.f; i += 0.1f) {
		drawTexturedQuad(i-fmod(camX, 0.1f), -camY, 0.1f, 0.05f);
	}
}

void drawBall() {
    res::ball.bind();
    glColor4f(1.f, 1.f, 1.f, 1.f);
	Point2 ballPoints[4] = {
							{ballXView-ballSize-camX, ballYView-camY}, 
							{ballXView+ballSize-camX, ballYView-camY}, 
							{ballXView+ballSize-camX, ballYView-camY+ballSize*2}, 
							{ballXView-ballSize-camX, ballYView-camY+ballSize*2}, 
						   };
    //drawTexturedQuad(ballXView-ballSize-camX, ballYView-camY, ballSize*2, ballSize*2);
	glBegin(GL_QUADS);
	for(int i = 0; i < 4; i++) {
		glTexCoord2f(i < 2 ? 1.f : 0.f, i%3 == 0 ? 0.f : 1.f);
		plotPoint(rotate(ballPoints[i], ballAngle, ballXView-camX, ballYView-camY+ballSize));
	}
	glEnd();
}

void PlayState::draw() {
	float blue = 1.f-(max(0.001f, ballYView-10)/3000.f);
	clear(blue/3.f, blue/3.f, blue);
	drawBackground();
    drawBall();
	
	
	if(state == "aiming") {
		drawBar();
		drawCenteredString(to_string((int)saves::parts["hits"]) + " Hits Left", 0.f, 0.9f, 1.5f);
		drawCenteredString("Press s to go to the shop", 0.f, -0.7f, 0.7f);
	} else if(state == "playing") {
		drawBar();
	} else if(state == "transitioningToEnd" || state == "end") {
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		if(state == "transitioningToEnd") glColor4f(0.f, 0.f, 0.f, (float)min(50, framesSinceStateChange)/100.f);
		else glColor4f(0.f, 0.f, 0.f, 0.5f);
		
		glVertex2f((16.f/9.f), 1.f);
		glVertex2f(-(16.f/9.f), 1.f);
		glVertex2f(-(16.f/9.f), -1.f);
		glVertex2f((16.f/9.f), -1.f);
		
		glEnd();
	} else if(state == "levelUp") {
		glBegin(GL_QUADS);
		glColor4f(1.f, 0.f, 0.f, 1.f);
		
		glVertex2f((16.f/9.f), 1.f);
		glVertex2f(-(16.f/9.f), 1.f);
		glVertex2f(-(16.f/9.f), -1.f);
		glVertex2f((16.f/9.f), -1.f);
		
		glEnd();
		drawCenteredString("Level Up!", 0.f, 0.6f, 3.f, sin(glfwGetTime()), cos(glfwGetTime()), sin(glfwGetTime()+2.f));
		drawCenteredString(doubleToString(saves::masterSaveParts["xpLevel"]), 0.f, -0.2f, 4.f);
		drawCenteredString("Click to continue", 0.f, -0.7f, 0.7f);
	}
	if(state == "end") {
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		
		glVertex2f(0.5f, 0.6f);
		glVertex2f(-0.5f, 0.6f);
		glVertex2f(-0.5f, -0.6f);
		glVertex2f(0.5f, -0.6f);
		
		glColor3f(1.f, 0.f, 0.f);
		
		glVertex2f(0.45f, -0.55f);
		glVertex2f(-0.45f, -0.55f);
		glVertex2f(-0.45f, -0.4f);
		glVertex2f(0.45f, -0.4f);
		
		glVertex2f(0.45f, -0.35f);
		glVertex2f(-0.45f, -0.35f);
		glVertex2f(-0.45f, -0.2f);
		glVertex2f(0.45f, -0.2f);
		
		glVertex2f(0.45f, -0.15f);
		glVertex2f(-0.45f, -0.15f);
		glVertex2f(-0.45f, -0.f);
		glVertex2f(0.45f, -0.f);
		
		glEnd();
		
		drawCenteredString("Play Again", 0.f, -0.475f, 1.f, 1.f, 0.f, 0.f);
		drawCenteredString("Exit", 0.f, -0.275f, 1.f, 1.f, 0.f, 0.f);
		drawCenteredString("To the Shop!", 0.f, -0.075f, 1.f, 1.f, 0.f, 0.f);
	}
	
	if(resetInt > 0) {
		if(resetFrames > 25 && resetInt == 1) {
			resetInt = 2;
			reset();
		}
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor4f(1.f, 1.f, 1.f, 1.0f);
		
		float offset = (float)(resetFrames/6.f)-0.4f;
		
		glVertex2f(-(16.f/9.f)*3.5f+offset, 1.f);
		glVertex2f(-(16.f/9.f)*3.5f+offset+0.3f, -1.f);
		glVertex2f(-(16.f/9.f)+offset+0.3f, -1.f);
		glVertex2f(-(16.f/9.f)+offset, 1.f);
		
		glEnd();
		if(offset > 15.f) {
			resetInt = 0;
		}
	}
}

void play() {
	changeState("playing");
	float baseLaunchPower = (float)(1000-abs(pointerPlace))/1000.f;
	launchPower = baseLaunchPower * saves::parts["power"];
	ballXVel = launchPower;
	ballYVel = launchPower;
	framesSinceBouncing = 0;
	saves::parts["hits"]--;
}

void singleUpdate() {
	if(state == "playing") {
		ballY += getYChange();
		ballX += getXChange();
		ballYVel -= max(abs(ballYVel)/1000.f, 0.0004f+abs(min(ballY/1000000.f, 10.f)));
		if(ballY < 0.f) {
			ballY = 0.f;
			ballYVel = -ballYVel/1.75f;
			if(ballYVel <= 0.005f) {
				ballYVel = 0.f;
			}
		} 
		if(ballY <= 0.f) {
			if(framesSinceBouncing > 5 && framesSinceBouncing != 0) {
				ballXVel /= 1.1f;
				ballAngleVel = fmod(ballX*7738.f, 10.f)-5.f;
			} else {
				ballXVel /= 1.003f;
				ballAngleVel = ballXVel*-750.f;
			}
			framesSinceBouncing = 0;
		}
	}
	
	ballAngle += ballAngleVel/10.f;
}

void PlayState::unfixedUpdate() {
	if(isPaused) return;
	if(state == "aiming") {
		singleUpdate();
		pointerPlace = (int)(sin(glfwGetTime()*3.f)*1000.f);
	} else if(state == "playing") {
		/*ballXView = ballX+((ballXVel)*progressThroughFrame);
		ballYView = ballY+((ballYVel)*progressThroughFrame);*/
		ballXView = ballX+xFrameChange()*progressThroughFrame;
		ballYView = ballY+yFrameChange()*progressThroughFrame;
	}
	camX = ballXView-(-16.f/9.f+0.6f);
	camY = max(1.f, ballYView-0.2f);
}

void PlayState::update() {
	targetFixedFps = 0.5f;
	if(state == "playing") {
		if(isPaused) return;
		framesSinceBouncing++;
		for(int i = 0; i < (int)max(1.f, 8*speed); i++) {
			singleUpdate();
		}
		if(ballXVel < 0.001f) {
			isStopped = true;
		}
		if(isStopped) {
			framesStopped++;
			speed = 1.f;
		}
		if(framesStopped >= 45) {
			saves::save(saves::getSavePath());
			saves::masterSaveParts["xp"] += (double)(ballX/3.f);
			if(saves::masterSaveParts["xp"] >= saves::getXPforLevel((int)saves::masterSaveParts["xpLevel"])) {
				while(saves::masterSaveParts["xp"] >= saves::getXPforLevel()) {
					saves::masterSaveParts["xp"] -= saves::getXPforLevel((int)saves::masterSaveParts["xpLevel"]);
					saves::masterSaveParts["xpLevel"]++;
				}
				changeState("levelUp");
			} else {
				changeState("transitioningToEnd");
			}
			if(ballX > saves::masterSaveParts["bestX"]) {
				saves::masterSaveParts["bestX"] = ballX;
				saves::masterSave();
			}
		}
		ballXView = ballX;
		ballYView = ballY;
	} else if(state == "transitioningToEnd") {
		if(framesSinceStateChange == 45) {
			changeState("end");
		}
	}
	framesSinceStateChange++;
	resetFrames++;
}
void goTilEnd() {
	while(framesStopped == 0) {
		singleUpdate();
	}
}
void PlayState::onKeyPress(int key, int action, int mods) {
	if(state == "aiming") {
		if(key == GLFW_KEY_SPACE) {
			play();
		} else if(key == GLFW_KEY_S) {
			goToState(new ShopState());
		}
	} else if(state == "playing") {
		if(key == GLFW_KEY_ESCAPE) {
			isPaused = !isPaused;
		} else if(key == GLFW_KEY_RIGHT) {
			speed *= 2.f;
			speed = min(speed, 100.f);
		} else if(key == GLFW_KEY_LEFT) {
			speed /= 2.f;
			speed = max(speed, 0.125f);
		}
	}
	
}
void PlayState::onMouseClick(double x, double y, int action, int button) {
	if(state == "end") {
		if(x > -0.45d && x < 0.45d && y > -0.55d && y < -0.4d) {
			resetInt = 1;
			resetFrames = 0;
		} else if(x > -0.45d && x < 0.45d && y > -0.35d && y < -0.2d) {
			exitToParent();
		} else if(x > -0.45d && x < 0.45d && y > -0.15d && y < -0.d) {
			goToState(new ShopState());
		}
	} else if(state == "levelUp") {
		changeState("end");
	}
}
string PlayState::getDebugMenu() {
	return "Current FPS: " + to_string(currentFps) + "\nballX: " + to_string(ballX) + "\nballY: " + to_string(ballY) + 
		"\nballXVel: " + to_string(ballXVel) +" \nballYVel: " + to_string(ballYVel) + "\nLaunch Power: " + to_string(launchPower) + 
		"\nstate: " + state + "\nbestX: " + to_string(saves::masterSaveParts["bestX"]) + "\nSpeed: " + to_string(speed) + 
		"    UPF: " + to_string(speed*8.f) + "\nPTF: " + to_string(progressThroughFrame);
}
void PlayState::postProcessing() {
	
}
void PlayState::onLoad() {
	
}
void PlayState::onUnload() {
	reset();
}