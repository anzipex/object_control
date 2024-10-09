#pragma once

#include <iostream>

class Control {
public:
    Control();
    virtual ~Control();
    static int randomPosition();
    int signChangeRotate() const;
    int signChangeTranslate() const;
    void changeRotate();
    void changeTranslate();
    void setAlwaysRotate();
    void setAlwaysTranslate();
    void clearAll();
    void printInfo() const;

    const float _stepUp;
    const float _stepDown;
    const float _stepLeft;
    const float _stepRight;

    float _rotate;
    float _rotateSwitch;

    bool _switcherRotate = false;
    float _rotateCounter;
    float _doRotate;

    float _translateX;
    float _translateY;
    float _translateSwitch;
    bool _switcherTranslate;
    float _translateCounter;
    float _doTranslate;

    bool _movement;

private:
    bool _randomRotate;
    bool _randomTranslate;
};
