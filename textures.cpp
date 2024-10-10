#include <fstream>
#include <memory>
#include <stdexcept>
#include <GL/gl.h>

#include "textures.h"
#include "pngutil.h"

Texture::Texture(const char *filename) :
texture_(0) {
    loadFromFile(filename);
}

Texture::~Texture() {
    if (texture_) {
        glDeleteTextures(1, &texture_);
    }
}

void Texture::loadFromFile(const std::string &filename) {
    FILE *fileHandle = fopen(filename.c_str(), "rb");
    if (!fileHandle) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::unique_ptr<FILE, decltype(&fclose)> file(fileHandle, fclose);

    if (pnguIsPng(file.get()) == 0) {
        throw std::runtime_error("File is not a valid PNG: " + filename);
    }

    int bytesPerPixel = 0;

    pnguReadHeader(file.get(), &width_, &height_, &bytesPerPixel);

    if (bytesPerPixel != 4) {
        throw std::runtime_error("File is not RGBA_8888: " + filename);
    }

    auto pixels = readPixels(file.get());

    createOpenglTexture(pixels.data());
}

std::vector< unsigned char > Texture::readPixels(FILE *file) const {
    constexpr int bytesPerPixel = 4;
    std::vector<unsigned char> pixels(width_ * height_ * bytesPerPixel);
    pnguReadData(file, pixels.data());
    return pixels;
}

void Texture::createOpenglTexture(const unsigned char *pixels) {
    glGenTextures(1, &texture_);

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture_);
}

int Texture::width() const {
    return width_;
}

int Texture::height() const {
    return height_;
}