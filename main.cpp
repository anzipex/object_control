/** 
 * @file main.cpp
 * @brief initial_dist
 * @author a.akulin
 * @date February 10, 2018
 */

#include <iostream>
#include <chrono>
#include <GL/glut.h>
#include "Object.h"

namespace {
Object *myobject = nullptr;
}

uint64_t GetMillisec();
void KeyboardFunc(unsigned char key, int x, int y);
void SpecialFunc(int key, int x, int y);
void ReshapeFunc(int width, int height);
void ClearAll();
int RandomPosition();
int SignChanger();
void ChangeRotate();

uint64_t prevUpdateTime = GetMillisec();
float updatePerSecond = 60;
float accumulator = 0;
uint64_t prevFrameTime = GetMillisec();
float framesPerSecond = 60;
uint64_t prevInputTime = GetMillisec();
float inputPerSecond = 200;

/* control */
const float StepUp = 10.0f;
const float StepDown = 10.0f;
const float StepLeft = 10.0f;
const float StepRight = 10.0f;

static float Rotate;
static float RotateSwitch;
static float RotateCounter = 0;
static int DoRotate = 0;
bool SwitcherRotate = false;
bool RandomRotate = false;

static float TranslateX;
static float TranslateY;

/* end of control */

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

void PrintInfo() {
    std::cout << "up: " << StepUp << " | " <<
        "down: " << StepDown << " | " <<
        "left: " << StepLeft << " | " <<
        "right: " << StepRight << " | " <<
        "translateX: " << TranslateX << " | " <<
        "translateY: " << TranslateY << " | " <<
        "rotate: " << Rotate << std::endl;
}

void ClearAll() {
    Rotate = 0;
    DoRotate = 0;
    TranslateX = 0;
    TranslateY = 0;
}

int SignChanger() {
    float sign;
    if (RandomRotate) {
        sign = 1.0f;
    } else {
        sign = -1.0f;
    }
    return sign;
}

void ChangeRotate() {
    if (RandomRotate) {
        RandomRotate = false;
    } else {
        RandomRotate = true;
    }
}

void IdleFunc() {
    const auto time = GetMillisec();
    auto updateTimeExp = time - prevUpdateTime;

    while (updateTimeExp > (1000 / updatePerSecond)) {

        if (SwitcherRotate) {
            DoRotate = DoRotate + (SignChanger() * (RotateCounter + 1.0f));
            RotateSwitch = DoRotate;
            myobject->setTranslate(TranslateX, TranslateY);
            myobject->setRotate(RotateSwitch);
            myobject->setDistance(TranslateX, TranslateY);
        } else {
            DoRotate = 0;
            RotateCounter = 0;
            myobject->setTranslate(TranslateX, TranslateY);
            myobject->setRotate(Rotate);
            myobject->setDistance(TranslateX, TranslateY);
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
    glutReshapeWindow(1280, 720);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    //    gluPerspective(45, 16 / 9, 1, 1000);
    glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, 0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void Display(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Object Control");
    glutDisplayFunc(DisplayFunc);
    glutIdleFunc(IdleFunc);
    glutReshapeFunc(ReshapeFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int RandomPosition() {
    int output = -400 + (rand() % static_cast<int>(400 - (-400) + 1));
    return output;
}

void KeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
            /* num 8 (up) */
        case 56:
            TranslateY += StepUp;
            PrintInfo();
            break;

            /* num 2 (down) */
        case 50:
            TranslateY -= StepDown;
            PrintInfo();
            break;

            /* num 4 (left) */
        case 52:
            TranslateX -= StepLeft;
            PrintInfo();
            break;

            /* num 6 (right) */
        case 54:
            TranslateX += StepRight;
            PrintInfo();
            break;

            /* num 7 (up left) */
        case 55:
            TranslateY += StepUp;
            TranslateX -= StepLeft;
            PrintInfo();
            break;

            /* num 9 (up right) */
        case 57:
            TranslateY += StepUp;
            TranslateX += StepRight;
            PrintInfo();
            break;

            /* num 1 (down left) */
        case 49:
            TranslateY -= StepDown;
            TranslateX -= StepLeft;
            PrintInfo();
            break;

            /* num 3 (down right) */
        case 51:
            TranslateY -= StepDown;
            TranslateX += StepRight;
            PrintInfo();
            break;

            /* num 0 (reset) */
        case 48:
            ClearAll();
            PrintInfo();
            break;

            /* num 5 (random) */
        case 53:
            std::cout << "random\n";
            TranslateX = RandomPosition();
            TranslateY = RandomPosition();
            Rotate = RandomPosition();
            PrintInfo();
            break;

            /* num plus (rotate) */
        case 43:
            if (SwitcherRotate) {
                SwitcherRotate = false;
                ChangeRotate();
            } else {
                SwitcherRotate = true;
            }
            break;

            /* escape (exit) */
        case 27:
            exit(1);
            break;
    }
}

void SpecialFunc(int key, int x, int y) {
    switch (key) {
            /* up */
        case GLUT_KEY_UP:
            TranslateY += StepUp;
            PrintInfo();
            break;

            /* down */
        case GLUT_KEY_DOWN:
            TranslateY -= StepDown;
            PrintInfo();
            break;

            /* rotate (left) */
        case GLUT_KEY_LEFT:
            Rotate += StepLeft;
            PrintInfo();
            break;

            /* rotate (right) */
        case GLUT_KEY_RIGHT:
            Rotate -= StepRight;
            PrintInfo();
            break;
    }
}

int main(int argc, char** argv) {
    Display(argc, argv);
    myobject = new Object;

    glutMainLoop();

    delete myobject;

    return 0;
}