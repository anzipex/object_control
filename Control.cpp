/** 
 * @file Control.cpp
 * @brief Implementing of class Control
 * @author a.akulin
 * @date February 20, 2018
 */

#include "Control.h"

Control::Control() :
_stepUp(10.0f),
_stepDown(10.0f),
_stepLeft(10.0f),
_stepRight(10.0f),
_randomRotate(false),
_switcherRotate(false),
_rotateCounter(0),
_doRotate(0),
_translateX(0),
_translateY(0),
_translateSwitch(0),
_randomTranslate(false),
_switcherTranslate(false),
_translateCounter(0),
_doTranslate(0),
_movement(true) {
}

Control::~Control() {
    clearAll();
}

int Control::randomPosition() {
    int output = -400 + (rand() % static_cast<int>(400 - (-400) + 1));
    return output;
}

int Control::signChangeRotate() {
    float sign;
    if (_randomRotate) {
        sign = 1.0f;
    } else {
        sign = -1.0f;
    }
    return sign;
}

int Control::signChangeTranslate() {
    float sign;
    if (_randomTranslate) {
        sign = 1.0f;
    } else {
        sign = -1.0f;
    }
    return sign;
}

void Control::changeRotate() {
    if (_randomRotate) {
        _randomRotate = false;
    } else {
        _randomRotate = true;
    }
}

void Control::changeTranslate() {
    if (_randomTranslate) {
        _randomTranslate = false;
    } else {
        _randomTranslate = true;
    }
}

void Control::setAlwaysRotate() {
    if (_switcherRotate) {
        _switcherRotate = false;
        changeRotate();
    } else {
        _switcherRotate = true;
    }
}

void Control::setAlwaysTranslate() {
    if (_switcherTranslate) {
        _switcherTranslate = false;
        changeTranslate();
    } else {
        _switcherTranslate = true;
    }
}

void Control::clearAll() {
    _rotate = 0;
    _doRotate = 0;
    _doTranslate = 0;
    _translateX = 0;
    _translateY = 0;
}

void Control::printInfo() {
    std::cout << "\033[1;32mUp\033[0m: " << _stepUp << " | " <<
        "\033[1;32mDown\033[0m: " << _stepDown << " | " <<
        "\033[1;32mLeft\033[0m: " << _stepLeft << " | " <<
        "\033[1;32mRight\033[0m: " << _stepRight << " | " <<
        "\033[1;33mTranslate\033[0m: (" << _translateX << ", " << _translateY << ")" << " | " <<
        "\033[1;33mRotate\033[0m: " << _rotate << std::endl;
}