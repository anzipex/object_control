/** 
 * @file Control.h
 * @brief Description of class Control
 * @author a.akulin
 * @date February 20, 2018
 */

#ifndef CONTROL_H
#define CONTROL_H

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
    bool _randomRotate = false;
    bool _switcherRotate = false;
    float _rotateCounter;
    float _doRotate;

    float _translateX;
    float _translateY;
    float _translateSwitch;
    bool _randomTranslate;
    bool _switcherTranslate;
    float _translateCounter;
    float _doTranslate;

    bool _movement;

private:
};

#endif /* CONTROL_H */