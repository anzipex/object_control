#pragma once

#include <iostream>

class Control {
public:
    Control();
    virtual ~Control();
    static int randomPosition();
    float signChangeRotate() const;
    float signChangeTranslate() const;
    void changeRotate();
    void changeTranslate();
    void setAlwaysRotate();
    void setAlwaysTranslate();
    void clearAll();
    void printInfo() const;

    const float stepUp_;
    const float stepDown_;
    const float stepLeft_;
    const float stepRight_;

    float rotate_;
    float rotateSwitch_;

    bool switcherRotate_ = false;
    float rotateCounter_;
    float doRotate_;

    float translateX_;
    float translateY_;
    float translateSwitch_;
    bool switcherTranslate_;
    float translateCounter_;
    float doTranslate_;

    bool movement_;

private:
    bool randomRotate_;
    bool randomTranslate_;
};
