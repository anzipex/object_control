#pragma once

#include <iostream>
#include <vector>

class Texture {
public:
    explicit Texture(const char *filename);
    ~Texture();

    void bind() const;
    int width() const;
    int height() const;

private:
    int width_;
    int height_;
    unsigned int texture_;

    void createOpenglTexture(const unsigned char *pixels);
    void loadFromFile(const std::string &filename);
    std::vector<unsigned char> readPixels(FILE *file) const;
};
