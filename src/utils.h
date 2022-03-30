#include <GLFW/glfw3.h>
#include <string>

#include "Image.h"

using namespace std;

#ifndef UTILS_H_
#define UTILS_H_

struct Point2 {
    float x;
    float y;
};

struct GameSettings {
	int hits;
	double money;
	float power;
	float gravity;
	float moneyGain;
	string savePath;
};

#endif

double getMouseX();
double getMouseY();
double getxPos(double xPos);
double getyPos(double yPos);
double getY();
double getX();
const char* readFile(char* file);
void checkShader(int shader);
void setMatrix(char* name, const float* data);
void setBool(const std::string &name, bool value);
void setInt(const std::string &name, int value);
void setFloat(const std::string &name, float value);
void setVec2(const std::string &name, float value1, float value2);
void setVec3(const std::string &name, float value1, float value2, float value3);
void setVec4(const std::string &name, float value1, float value2, float value3, float value4);
int shaders(string vs, string fs);
int shaders();
Image loadImage(std::string n, bool hasTrans);
void clear(float r, float g, float b);
Point2 rotate(Point2 in, float angle, float cx, float cy);
void plotPoint(Point2 in);
void fillOval(float x, float y, float width, float height, int subdivisions);
void drawTexturedQuad(float x, float y, float w, float h);
string doubleToString(double d);
void bangExit();