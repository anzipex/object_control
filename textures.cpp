/** 
 * @file textures.cpp
 * @brief SAMPLE_TEXT
 * @author a.akulin
 * @date January 25, 2018
 */

#include <iostream>
#include <GL/gl.h>

#include "textures.h"
#include "pngutil.h"

Texture::Texture(const char *filename) :
_texture(0) {
    loadFromFile(filename);
}

Texture::~Texture() {
    if (_texture) {
        glDeleteTextures(1, &_texture);
    }
}

void Texture::loadFromFile(const char * filename) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        std::cerr << filename << " can\'t open" << std::endl;
    }

    if (pnguIsPng(file) == 0) {
        std::cerr << filename << " is not PNG" << std::endl;
        fclose(file);
        return;
    }

    int bytesPerPixel = 0;
    pnguReadHeader(file, &_width, &_height, &bytesPerPixel);

    if (bytesPerPixel != 4) {
        std::cerr << filename << " is not RGBA_8888" << std::endl;
        fclose(file);
    }

    const auto pixels = readPixels(file);

    fclose(file);

    createOpenglTexture(pixels.data());
}

std::vector< unsigned char > Texture::readPixels(FILE *file) {
    constexpr int bytesPerPixel = 4;
    std::vector<unsigned char> pixels(_width * _height * bytesPerPixel);
    pnguReadData(file, pixels.data());
    return pixels;
}

void Texture::createOpenglTexture(const unsigned char *bytes) {
    glGenTextures(1, &_texture);

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _texture);
}

int Texture::width() const {
    return _width;
}

int Texture::height() const {
    return _height;
}