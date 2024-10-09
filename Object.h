#pragma once

#include "textures.h"

void InitTextError(const char *text, int length, int x, int y);
void DrawDebugRect(float width, float height);
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
    static void drawText(float positionX, float positionY, float angle);

    Texture _background;
    Texture _object;
    float _rotateAngle;
    float _translatePosX;
    float _translatePosY;
};
