#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stb_image.h>

#include "main.h"
#include "utils.h"
#include "handler.h"
#include "Text.h"
#include "Resources.h"
#include "Saves.h"

using namespace std;

GLFWwindow* createWindow(bool f);

bool isInfinite = false;

float width;
float height;

float pixelWidth;
float pixelHeight;

float xStretch;
float yStretch;

GLFWwindow* window;

int currentSecond = 0;
int framesSoFar = 0;
int currentFps = 0;
int totalFramesEver = 0;

float ratio, inverse;

float delta = 0.f;
float progressThroughFrame = 0.f;
float timeOfLastFrame = 0.f;
float targetFixedFps = 60.f;

static void enablings() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}
float lastFixedUpdate = 0.f;
static void update() {
	if(glfwGetTime()-lastFixedUpdate > 1.f/targetFixedFps) {
		handlerUpdate();
		lastFixedUpdate += 1.f/targetFixedFps;
	}
	progressThroughFrame = (glfwGetTime()-lastFixedUpdate)*targetFixedFps;
    handlerUnfixedUpdate();
}
static void draw() {
    handlerDraw();
}
static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}
bool testBool = false;
bool isFullscreen = false;
static void key_callback(GLFWwindow* eventWindow, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_F4 && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if((!action) == GLFW_PRESS) {
        return;
    }
    if(key == GLFW_KEY_SPACE) {
        testBool = !testBool;
    }
    onKey(key, action, mods);
}
static void window_size_callback(GLFWwindow* window, int width, int height) {
    textReInit();
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS) onMouse(getX(), getY(), action, button);
}
void calculateFPS() {
    totalFramesEver++;
    if(floor(glfwGetTime()) > currentSecond) {
        currentSecond = floor(glfwGetTime());
        currentFps = framesSoFar;
        framesSoFar = 0;
        //glfwSetWindowTitle(window, ("Current fps: " + to_string(currentFps)).c_str());
    } else {
        framesSoFar++;
    }
}
float getHBorder() {
    if(ratio > (16.f/9.f)) {
        return ratio;
    } else {
        return (16.f/9.f);
    }
}
float getYBorder() {
    if(ratio > (16.f/9.f)) {
        return 1.f;
    } else {
        return (16.f/9.f) * inverse;
    }
}
GLFWwindow* createWindow(bool f) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	const GLFWvidmode* v = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GLFWwindow* e;
	if(!f) {
		e =  glfwCreateWindow(v->width, v->height, "Bang", NULL, NULL);
	} else {
		e =  glfwCreateWindow(v->width, v->height, "Bang", glfwGetPrimaryMonitor(), NULL);
	}
	if (!e)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(e);
    glfwSetKeyCallback(e, key_callback);
    glfwSetMouseButtonCallback(e, mouse_button_callback);
    glfwSetWindowSizeCallback(e, window_size_callback);
    glfwSwapInterval(1);
	
	return e;
}
void resetMatrix() {
	int iwidth, iheight;
	glfwGetFramebufferSize(window, &iwidth, &iheight);
	ratio = (float) iwidth / (float) iheight;
	inverse = (float)iheight / (float)iwidth;
	//glViewport(0, 0, iwidth, iheight);
	glViewport(0, 0, iwidth, iheight);
	//glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-getHBorder(), getHBorder(), -getYBorder(), getYBorder(), 1.f, -1.f);

	width = getHBorder()*2.f;
	height = getYBorder()*2.f;

	pixelWidth = (float)iwidth;
	pixelHeight = (float)iheight;

	if(ratio > (16.f/9.f)) {
		xStretch = getHBorder();
		yStretch = 1.f;
	} else {
		xStretch = (16.f/9.f);
		yStretch = getYBorder();
	}
	
	glUseProgram(0);
}
int main(int argc, char *argv[])
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    const GLFWvidmode* v = glfwGetVideoMode(glfwGetPrimaryMonitor());
    //window = glfwCreateWindow(v->width, v->height, "Bang", glfwGetPrimaryMonitor(), NULL);
    window = createWindow(true);
    //window = glfwCreateWindow(1600, 900, "Bang", NULL, NULL);
    

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    int textReturn = textInit();
    if(textReturn != 0) {
        cout << "Text initialization failed with error code " << textReturn << endl;
        return textReturn;
        cout << endl;
    }

    res::load();
	
	saves::init();
	saves::masterLoad();
	
	handOnLoad();

    while (!glfwWindowShouldClose(window))
    {
        calculateFPS();
        enablings();
		
		resetMatrix();
		
        textReInit();

        update();

        draw();
		
		delta = abs(glfwGetTime()-timeOfLastFrame);
        timeOfLastFrame = glfwGetTime();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	
	bangExit();
    return 0;
}
