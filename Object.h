/** 
 * @file Object.h
 * @brief Description of class Object
 * @author a.akulin
 * @date February 10, 2018
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "textures.h"

void InitTextError(const char *text, int length, int x, int y);
void DrawDebugRect(const float width, const float height);
void DrawTextureBg(Texture *texture);
void DrawTexture(Texture *texture);
void DrawRect(int width, int height);

class Object {
public:
    Object();
    void draw();
    void setRotate(float angle);
    void setTranslate(float positionX, float positionY);

private:
    void drawObject();
    void drawBackground();
    void drawText(float positionX, float positionY, float angle);

    Texture _background;
    Texture _object;
    float _rotateAngle;
    float _translatePosX;
    float _translatePosY;
};

#endif /* OBJECT_H */