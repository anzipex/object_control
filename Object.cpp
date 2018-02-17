/** 
 * @file Object.cpp
 * @brief Implementing of class Object 
 * @author a.akulin
 * @date February 10, 2018
 */

#include <iostream>
#include <string> 
#include <GL/glut.h>
#include "textures.h"
#include "Object.h"

void InitTextError(const char *text, int length, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    double matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void DrawDebugRect(const float width, const float height) {
    const auto hw = width / 2;
    const auto hh = height / 2;

    glBegin(GL_LINE_LOOP);

    glColor3ub(255, 0, 0);
    glVertex2f(hw, hh);

    glColor3ub(0, 255, 0);
    glVertex2f(-hw, hh);

    glColor3ub(0, 0, 255);
    glVertex2f(-hw, -hh);

    glColor3ub(255, 255, 255);
    glVertex2f(hw, -hh);

    glEnd();
}

void DrawRect(int width, int height) {
    const auto hw = width / 2;
    const auto hh = height / 2;

    glBegin(GL_QUADS);
    glVertex2f(hw, hh);
    glVertex2f(-hw, hh);
    glVertex2f(-hw, -hh);
    glVertex2f(hw, -hh);
    glEnd();
}

void DrawTexture(Texture *texture) {
    glEnable(GL_TEXTURE_2D);

    texture->bind();

    const auto hw = texture->width() / 2;
    const auto hh = texture->height() / 2;

    glBegin(GL_QUADS);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(hw, hh);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-hw, hh);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-hw, -hh);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(hw, -hh);

    glEnd();

    glDisable(GL_TEXTURE_2D);

#if 0
    DrawDebugRect(texture->width(), texture->height());
#endif
}

void DrawTextureBg(Texture* texture) {
    glEnable(GL_TEXTURE_2D);

    texture->bind();

    const auto hw = texture->width() / 2;
    const auto hh = texture->height() / 2;

    glBegin(GL_QUADS);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(hw, hh);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-hw, hh);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-hw, -hh);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(hw, -hh);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//--------------------------------------------------------------------------------------------------

Object::Object() :
_background("textures/templateBackground.png"),
_object("textures/templateObject.png"),
_rotateAngle(0),
_translatePosX(0),
_translatePosY(0) {
}

void Object::drawObject() {
    glPushMatrix();
    glTranslatef(_translatePosX, _translatePosY, 0);
    glRotatef(_rotateAngle, 0, 0, 1.0f);
    DrawTexture(&_object);
    glPopMatrix();
}

void Object::drawBackground() {
    glPushMatrix();
    DrawTextureBg(&_background);
    glPopMatrix();
}

void Object::drawText(float positionX, float positionY, float angle) {
    /* rotate */
    std::string strAngle = std::to_string(angle);
    std::string rotation = "rotation ";
    rotation.append(strAngle);
    InitTextError(rotation.data(), rotation.size(), 10, 45);

    /* translate */
    std::string strPosX = std::to_string(positionX);
    std::string strPosY = std::to_string(positionY);
    std::string distance = "translate from ceter ";
    distance.append(strPosX);
    distance.append(", ");
    distance.append(strPosY);
    InitTextError(distance.data(), distance.size(), 10, 30);

    /* copyright */
    std::string copyright = "made by anzipex";
    InitTextError(copyright.data(), copyright.size(), 10, 10);
    glutSwapBuffers();
}

void Object::draw() {
    drawBackground();
    drawObject();
    drawText(_translatePosX, _translatePosY, _rotateAngle);
}

void Object::setRotate(float rotate) {
    _rotateAngle = rotate;
}

void Object::setTranslate(float positionX, float positionY) {
    _translatePosX = positionX;
    _translatePosY = positionY;
}

void Object::setDistance(float positionX, float positionY) {
    _translatePosX = positionX;
    _translatePosY = positionY;
}