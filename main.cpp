/** 
 * @file main.cpp
 * @brief object_control
 * @author a.akulin
 * @date February 10, 2018
 */

#include <iostream>
#include <chrono>
#include <GL/glut.h>
#include "Object.h"
#include "Control.h"

namespace {
Object *myobject = nullptr;
Control *mycontrol = nullptr;
int WinWidth = 1280;
int WinHeight = 720;
bool Fullscreen = false;
}

uint64_t GetMillisec();
void KeyboardFunc(unsigned char key, int x, int y);
void SpecialFunc(int key, int x, int y);
void ReshapeFunc(int width, int height);
void SetFullscreen();

uint64_t prevUpdateTime = GetMillisec();
float updatePerSecond = 60;
float accumulator = 0;
uint64_t prevFrameTime = GetMillisec();
float framesPerSecond = 60;
uint64_t prevInputTime = GetMillisec();
float inputPerSecond = 200;

uint64_t GetMillisec() {
    using namespace std;
    using namespace chrono;

    const auto timePoint = high_resolution_clock::now();
    const auto duration = timePoint.time_since_epoch();
    return duration_cast<milliseconds>(duration).count();
}

void DisplayFunc() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    myobject->draw();

    glutSwapBuffers();
}

void IdleFunc() {
    const auto time = GetMillisec();
    auto updateTimeExp = time - prevUpdateTime;

    while (updateTimeExp > (1000 / updatePerSecond)) {

        if (mycontrol->_switcherRotate) {
            mycontrol->_doRotate = mycontrol->_doRotate + (mycontrol->signChangeRotate() * (mycontrol->_rotateCounter + 1.0f));
            mycontrol->_rotateSwitch = mycontrol->_doRotate;
            myobject->setRotate(mycontrol->_rotateSwitch);
        } else if (!mycontrol->_switcherRotate) {
            mycontrol->_doRotate = 0;
            mycontrol->_rotateCounter = 0;
            myobject->setRotate(mycontrol->_rotate);
        }

        if (mycontrol->_switcherTranslate) {
            mycontrol->_movement = true;
            mycontrol->_doTranslate = mycontrol->_doTranslate + (mycontrol->signChangeTranslate() * (mycontrol->_translateCounter + 1.0f));
            mycontrol->_translateSwitch = mycontrol->_translateX + mycontrol ->_doTranslate;
            myobject->setTranslate(mycontrol->_translateSwitch, mycontrol->_translateY);
        } else if (!mycontrol->_switcherTranslate) {
            if (mycontrol->_movement) {
                mycontrol->_translateX = mycontrol->_translateSwitch;
                mycontrol->_translateSwitch = 0;
                mycontrol->_doTranslate = 0;
                mycontrol->_movement = false;
            }
            myobject->setTranslate(mycontrol->_translateX, mycontrol->_translateY);
        }

        updateTimeExp -= 1000 / updatePerSecond;
        prevUpdateTime = time;
    }

    const auto frameTimeExp = time - prevFrameTime;

    if (frameTimeExp > (1000 / framesPerSecond)) {
        glutPostRedisplay();
        prevFrameTime = time;
    }

    const auto inputTimeExp = time - prevInputTime;
}

void ReshapeFunc(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, 0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void Display(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(WinWidth, WinHeight);
    glutInitWindowPosition(320, 180);
    glutCreateWindow("Object Control");
    glutDisplayFunc(DisplayFunc);
    glutIdleFunc(IdleFunc);
    glutReshapeFunc(ReshapeFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
            /* num 8 (up) */
        case 56:
            mycontrol->_translateY += mycontrol->_stepUp;
            mycontrol->printInfo();
            break;

            /* num 2 (down) */
        case 50:
            mycontrol->_translateY -= mycontrol->_stepDown;
            mycontrol->printInfo();
            break;

            /* num 4 (left) */
        case 52:
            mycontrol->_translateX -= mycontrol->_stepLeft;
            mycontrol->printInfo();
            break;

            /* num 6 (right) */
        case 54:
            mycontrol->_translateX += mycontrol->_stepRight;
            mycontrol->printInfo();
            break;

            /* num 7 (up left) */
        case 55:
            mycontrol->_translateY += mycontrol->_stepUp;
            mycontrol->_translateX -= mycontrol->_stepLeft;
            mycontrol->printInfo();
            break;

            /* num 9 (up right) */
        case 57:
            mycontrol->_translateY += mycontrol->_stepUp;
            mycontrol->_translateX += mycontrol->_stepRight;
            mycontrol->printInfo();
            break;

            /* num 1 (down left) */
        case 49:
            mycontrol->_translateY -= mycontrol->_stepDown;
            mycontrol->_translateX -= mycontrol->_stepLeft;
            mycontrol->printInfo();
            break;

            /* num 3 (down right) */
        case 51:
            mycontrol->_translateY -= mycontrol->_stepDown;
            mycontrol->_translateX += mycontrol->_stepRight;
            mycontrol->printInfo();
            break;

            /* num 0 (reset) */
        case 48:
            mycontrol->clearAll();
            mycontrol->printInfo();
            break;

            /* num 5 (random) */
        case 53:
            mycontrol->_translateX = mycontrol->randomPosition();
            mycontrol->_translateY = mycontrol->randomPosition();
            mycontrol->_rotate = mycontrol->randomPosition();
            mycontrol->_doTranslate = mycontrol->randomPosition();
            mycontrol->printInfo();
            break;

            /* num plus (rotate) */
        case 43:
            mycontrol->setAlwaysRotate();
            break;

            /* num minus (translate) */
        case 45:
            mycontrol->setAlwaysTranslate();
            break;

            /* escape (exit) */
        case 27:
            /* TODO: ask question y or n about exit */
            exit(1);
            /* end of TODO */
    }
}

void SpecialFunc(int key, int x, int y) {
    switch (key) {
            /* up */
        case GLUT_KEY_UP:
            mycontrol->_translateY += mycontrol->_stepUp;
            mycontrol->printInfo();
            break;

            /* down */
        case GLUT_KEY_DOWN:
            mycontrol->_translateY -= mycontrol->_stepDown;
            mycontrol->printInfo();
            break;

            /* rotate (left) */
        case GLUT_KEY_LEFT:
            mycontrol->_translateX -= mycontrol->_stepLeft;
            mycontrol->printInfo();
            break;

            /* rotate (right) */
        case GLUT_KEY_RIGHT:
            mycontrol->_translateX += mycontrol->_stepRight;
            mycontrol->printInfo();
            break;

            /* f11 (fullscreen) */
        case GLUT_KEY_F11:
            SetFullscreen();
            break;
    }
}

void SetFullscreen() {
    if (!Fullscreen) {
        glutFullScreen();
        Fullscreen = true;
    } else {
        glutReshapeWindow(WinWidth, WinHeight);
        Fullscreen = false;
    }
}

int main(int argc, char** argv) {
    Display(argc, argv);
    myobject = new Object;
    mycontrol = new Control();

    glutMainLoop();

    /* TODO: put in destructor, because never called */
    delete myobject;
    delete mycontrol;
    /* END OF TODO */

    return 0;
}