/** 
 * @file textures.h
 * @brief SAMPLE_TEXT
 * @author a.akulin
 * @date January 25, 2018
 */

#ifndef TEXTURES_H
#define TEXTURES_H

#include <vector>

using uchar = unsigned char;

/**
 * @brief Работа с текстурами
 */
class Texture {
public:
    explicit Texture(const char *filename);
    ~Texture();

    /**
     * @brief Привязка текстуры
     */
    void bind() const;
    
    /**
     * @brief Ширина текстуры
     * @return ширина
     */
    int width() const;
    
    /**
     * @brief Высота текстуры
     * @return высота
     */
    int height() const;

private:
    int _width; //!< ширина текстуры
    int _height; //!< высота текстуры
    unsigned int _texture; //!< текстура

    void createOpenglTexture(const unsigned char *pixels); //!< создание текстуры
    void loadFromFile(const char *filename); //!< загрузка текстуры из файла
    std::vector< unsigned char > readPixels(FILE *file); //!< вектор пикселей
};

#endif /* TEXTURES_H */