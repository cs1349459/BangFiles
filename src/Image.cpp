#include "Image.h"
#include <glad/glad.h>

void Image::bind() {
   glBindTexture(GL_TEXTURE_2D, this->id);
}