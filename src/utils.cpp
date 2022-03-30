#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <sstream>

#include "utils.h"
#include "Image.h"
#include "main.h"
#include "Saves.h"

using namespace std;

int vShader;
int fShader;
int shaderProgram;
int vao;

float degreesToRadians(float degree) {
    return ((degree * 3.14159) / 180.f);
}

double getMouseX() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return xpos;
}

double getMouseY() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return ypos;
}

double getxPos(double xPos) {
    return ((xPos-(pixelWidth/2))/(pixelWidth/2))*xStretch;
}

double getyPos(double yPos) {
    return ((yPos-(pixelHeight/2))/(pixelHeight/2))*yStretch;
}

double getX() {
    return getxPos(getMouseX());
}

double getY() {
    return -getyPos(getMouseY());
}

const char* readFile(char* file) {
    fstream newfile;
    newfile.open(file,ios::in);
    string toReturn;
    if (newfile.is_open()) {
        string tp;
        while(getline(newfile, tp)) {
            toReturn += tp + "\n";
        }
        newfile.close();
    }
    const char* e = toReturn.c_str();
    return e;
}

void checkShader(int shader) {
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
    }
}

void setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}
void setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void setVec2(const std::string &name, float value1, float value2) {
    glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), value1, value2);
}
void setVec3(const std::string &name, float value1, float value2, float value3) {
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value1, value2, value3);
}
void setVec4(const std::string &name, float value1, float value2, float value3, float value4) {
    glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value1, value2, value3, value4);
}

int shaders(string vs, string fs) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vs);
        fShaderFile.open(fs);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glShaderSource(vertexShader, 1, (const GLchar* const*)readFile((char*)"shaders/vertex.vs"), NULL);
    glCompileShader(vertexShader);
    checkShader(vertexShader);
    vShader = vertexShader;

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glShaderSource(fragmentShader, 1, (const GLchar* const*)readFile((char*)"shaders/fragment.fs"), NULL);
    glCompileShader(fragmentShader);
    checkShader(fragmentShader);
    fShader = fragmentShader;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    return shaderProgram;
}

int shaders() {
    return shaders("shaders/vertex.vs", "shaders/fragment.fs");
}

Image loadImage(string source, bool hasTrans) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(source.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if(hasTrans) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        Image i;
        i.id = 0;
        i.width = 0;
        i.height = 0;
        return i;
    }
    stbi_image_free(data);
    Image i;
    i.id = texture;
    i.width = width;
    i.height = height;
    return i;
}

void clear(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Point2 rotate(Point2 p, float ang, float cx, float cy) {
    float angle = degreesToRadians(ang);
    return {cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
            sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy};
}
void plotPoint(Point2 in) {
    glVertex2f(in.x, in.y);
}
void fillOval(float x, float y, float width, float height, int subdivisions) {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    for(int ii = 0; ii < subdivisions; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(subdivisions);//get the current angle

        float cx = width * cosf(theta);//calculate the x component
        float cy = height * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}
void doXVertices(float x, float y, float z) {
   float vertices[] = {
       // positions         // colors           // texture coords
      x,     y+1.f, z+1.f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
      x,     y,     z+1.f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
      x,     y,     z,       0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
      x,     y+1.f, z,       1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
   };
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void drawTexturedQuad(float x, float y, float w, float h) {
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glTexCoord2f(1.f, 0.f);
    glVertex2f(x+w, y+h);
    glTexCoord2f(1.f, 1.f);
    glVertex2f(x+w, y);
    glTexCoord2f(0.f, 1.f);
    glVertex2f(x, y);
    glTexCoord2f(0.f, 0.f);
    glVertex2f(x, y+h);

    glEnd();
}

string doubleToString(double d) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << d;
	return stream.str();
}

void bangExit() {
	saves::masterSave();
	
    glfwDestroyWindow(window);
    glfwTerminate();
}