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
Object *object = nullptr;
Control *control = nullptr;
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

    object->draw();

    glutSwapBuffers();
}

void IdleFunc() {
    const auto time = GetMillisec();
    auto updateTimeExp = time - prevUpdateTime;

    while (updateTimeExp > (1000 / updatePerSecond)) {

        if (control->_switcherRotate) {
            control->_doRotate = control->_doRotate + (control->signChangeRotate() *
                                                       (control->_rotateCounter + 1.0f));
            control->_rotateSwitch = control->_doRotate;
            object->setRotate(control->_rotateSwitch);
        } else if (!control->_switcherRotate) {
            control->_doRotate = 0;
            control->_rotateCounter = 0;
            object->setRotate(control->_rotate);
        }

        if (control->_switcherTranslate) {
            control->_movement = true;
            control->_doTranslate = control->_doTranslate + (control->signChangeTranslate() *
                                                             (control->_translateCounter + 1.0f));
            control->_translateSwitch = control->_translateX + control ->_doTranslate;
            object->setTranslate(control->_translateSwitch, control->_translateY);
        } else if (!control->_switcherTranslate) {
            if (control->_movement) {
                control->_translateX = control->_translateSwitch;
                control->_translateSwitch = 0;
                control->_doTranslate = 0;
                control->_movement = false;
            }
            object->setTranslate(control->_translateX, control->_translateY);
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
            control->_translateY += control->_stepUp;
            control->printInfo();
            break;

            /* num 2 (down) */
        case 50:
            control->_translateY -= control->_stepDown;
            control->printInfo();
            break;

            /* num 4 (left) */
        case 52:
            control->_translateX -= control->_stepLeft;
            control->printInfo();
            break;

            /* num 6 (right) */
        case 54:
            control->_translateX += control->_stepRight;
            control->printInfo();
            break;

            /* num 7 (up left) */
        case 55:
            control->_translateY += control->_stepUp;
            control->_translateX -= control->_stepLeft;
            control->printInfo();
            break;

            /* num 9 (up right) */
        case 57:
            control->_translateY += control->_stepUp;
            control->_translateX += control->_stepRight;
            control->printInfo();
            break;

            /* num 1 (down left) */
        case 49:
            control->_translateY -= control->_stepDown;
            control->_translateX -= control->_stepLeft;
            control->printInfo();
            break;

            /* num 3 (down right) */
        case 51:
            control->_translateY -= control->_stepDown;
            control->_translateX += control->_stepRight;
            control->printInfo();
            break;

            /* num 0 (reset) */
        case 48:
            control->clearAll();
            control->printInfo();
            break;

            /* num 5 (random) */
        case 53:
            control->_translateX = control->randomPosition();
            control->_translateY = control->randomPosition();
            control->_rotate = control->randomPosition();
            control->_doTranslate = control->randomPosition();
            control->printInfo();
            break;

            /* num plus (rotate) */
        case 43:
            control->setAlwaysRotate();
            break;

            /* num minus (translate) */
        case 45:
            control->setAlwaysTranslate();
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
            control->_translateY += control->_stepUp;
            control->printInfo();
            break;

            /* down */
        case GLUT_KEY_DOWN:
            control->_translateY -= control->_stepDown;
            control->printInfo();
            break;

            /* rotate (left) */
        case GLUT_KEY_LEFT:
            control->_translateX -= control->_stepLeft;
            control->printInfo();
            break;

            /* rotate (right) */
        case GLUT_KEY_RIGHT:
            control->_translateX += control->_stepRight;
            control->printInfo();
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

    object = new Object;
    control = new Control();

    glutMainLoop();

    /* TODO: put in destructor, because never called */
    delete object;
    delete control;
    /* END OF TODO */

    return 0;
}