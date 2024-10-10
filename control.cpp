#include "control.h"

Control::Control() :
stepUp_(10.0F),
stepDown_(10.0F),
stepLeft_(10.0F),
stepRight_(10.0F),
rotate_(0),
rotateSwitch_(0),
randomRotate_(false),
rotateCounter_(0),
doRotate_(0),
translateX_(0),
translateY_(0),
translateSwitch_(0),
randomTranslate_(false),
switcherTranslate_(false),
translateCounter_(0),
doTranslate_(0),
movement_(false) {
}

Control::~Control() {
    clearAll();
}

int Control::randomPosition() {
    return (-400 + (rand() % static_cast<int>(400 - (-400) + 1)));
}

float Control::signChangeRotate() const {
    if (randomRotate_) {
        return 1.0F;
    }
    return -1.0F;
}

float Control::signChangeTranslate() const {
    if (randomTranslate_) {
        return 1.0F;
    }
    return -1.0F;
}

void Control::changeRotate() {
    randomRotate_ = !randomRotate_;
}

void Control::changeTranslate() {
    randomTranslate_ = !randomTranslate_;
}

void Control::setAlwaysRotate() {
    if (switcherRotate_) {
        switcherRotate_ = false;
        changeRotate();
    } else {
        switcherRotate_ = true;
    }
}

void Control::setAlwaysTranslate() {
    if (switcherTranslate_) {
        switcherTranslate_ = false;
        changeTranslate();
    } else {
        switcherTranslate_ = true;
    }
}

void Control::clearAll() {
    rotate_ = 0;
    doRotate_ = 0;
    doTranslate_ = 0;
    translateX_ = 0;
    translateY_ = 0;
}

void Control::printInfo() const {
    std::cout << "\033[1;32mUp\033[0m: " << stepUp_ << " | "
              << "\033[1;32mDown\033[0m: " << stepDown_ << " | "
              << "\033[1;32mLeft\033[0m: " << stepLeft_ << " | "
              << "\033[1;32mRight\033[0m: " << stepRight_ << " | "
              << "\033[1;33mTranslate\033[0m: (" << translateX_ << ", " << translateY_ << ")"
              << " | "
              << "\033[1;33mRotate\033[0m: " << rotate_ << '\n';
}
