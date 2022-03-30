#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern float width;
extern float height;

extern float pixelWidth;
extern float pixelHeight;

extern GLFWwindow* window;

extern float xStretch;
extern float yStretch;

extern float ratio, inverse;

extern bool testBool;

extern int currentFps;
extern int totalFramesEver;

float getHBorder();
float getYBorder();

void resetMatrix();

extern float delta;
extern float progressThroughFrame;
extern float targetFixedFps;
