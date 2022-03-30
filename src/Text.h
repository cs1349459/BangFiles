#pragma once

void textReInit();
int textInit();
float getStringWidth(std::string in, float scale);
void drawString(std::string text, float x, float y, float scale, float r, float g, float b);
void drawString(std::string text, float x, float y, float scale);
void drawCenteredString(std::string text, float x, float y, float scale);
void drawCenteredString(std::string text, float x, float y, float scale, float r, float g, float b);