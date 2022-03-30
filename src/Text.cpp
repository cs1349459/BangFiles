#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#define GLT_IMPLEMENTATION
#include "gltext.h"

#include "main.h"

using namespace std;

GLTtext* text1;

int textInit() {
    if(!gltInit()) {
        return -1;
    }

    text1 = gltCreateText();

    return 0;
}

void textReInit() {
    gltViewport((int)pixelWidth, (int)pixelHeight);
}

float getStringWidth(std::string in, float scale) {
    gltSetText(text1, in.c_str());
    return gltGetTextWidth(text1, scale);
}

float getStringHeight(std::string in, float scale) {
    gltSetText(text1, in.c_str());
    return gltGetTextHeight(text1, scale);
}

void drawStringRaw(std::string text, float x, float y, float scale, float r, float g, float b) {
    if(ratio <= (16.f/9.f)) {
        scale *= pixelWidth/1920.f;
    } else {
        scale *= pixelHeight/1080.f;
    }

    gltBeginDraw();

    gltSetText(text1, text.c_str());

    gltColor(r, g, b, 1.0f);
    gltDrawText2D(text1, x, y, scale*1.5f);

    /*gltColor(
    	cosf((float)glfwGetTime()) * 0.5f + 0.5f,
    	sinf((float)glfwGetTime()) * 0.5f + 0.5f,
    	1.0f,
    	1.0f);*/

    //gltDrawText2DAligned(text1, 0.0f, pixelHeight, 1.0f, GLT_LEFT, GLT_BOTTOM);

    gltEndDraw();
}

void drawString(std::string text, float x, float y, float scale, float r, float g, float b) {
    y /= -2;
	x /= 2;

    x += xStretch/2;
    x *= pixelWidth/xStretch;

    y += yStretch/2;
    y *= pixelHeight/yStretch;

    drawStringRaw(text, x, y, scale, r, g, b);
}

void drawString(std::string text, float x, float y, float scale) {
    drawString(text, x, y, scale, 1.f, 1.f, 1.f);
}

void drawCenteredString(std::string text, float x, float y, float scale, float r, float g, float b) {
    y /= -2;
	x /= 2;

    x += xStretch/2;
    x *= pixelWidth/xStretch;

    y += yStretch/2;
    y *= pixelHeight/yStretch;

    x -= getStringWidth(text, scale)/2;
    y -= getStringHeight(text, scale)/2;

    //if(testBool) y -= (pixelHeight/2)-((pixelHeight/2)-((pixelHeight/2)/yStretch));

    drawStringRaw(text, x, y, scale, r, g, b);
}

void drawCenteredString(std::string text, float x, float y, float scale) {
    drawCenteredString(text, x, y, scale, 1.f, 1.f, 1.f);
}
