#include <chrono>
#include <memory>
#include <GL/glut.h>

#include "object.h"
#include "control.h"

namespace {
const int WinWidth = 1280;
const int WinHeight = 720;
bool Fullscreen = false;
bool ExitPrompt = false;
} // namespace

uint64_t GetMillisec();
void KeyboardFunc(unsigned char key, int x, int y);
void SpecialFunc(int key, int x, int y);
void ReshapeFunc(int width, int height);
void SetFullscreen();

static std::unique_ptr<Object> object = nullptr;
static std::unique_ptr<Control> control = nullptr;

uint64_t PrevUpdateTime = GetMillisec();
const float UpdatePerSecond = 60;
uint64_t PrevFrameTime = GetMillisec();
float FramesPerSecond = 60;

uint64_t GetMillisec() {
    const auto timePoint = std::chrono::high_resolution_clock::now();
    const auto duration = timePoint.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void DisplayFunc() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (ExitPrompt) {
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(-56.0F, 12.0F);
        const char* message = "Y or N to exit";
        for (const char* c = message; *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        glutSwapBuffers();
        return;
    }

    glLoadIdentity();

    object->draw();

    glutSwapBuffers();
}

void IdleFunc() {
    const auto time = GetMillisec();
    auto updateTimeExp = time - PrevUpdateTime;

    while (static_cast<float>(updateTimeExp) > (1000 / UpdatePerSecond)) {

        if (control->switcherRotate_) {
            control->doRotate_ = control->doRotate_ + (control->signChangeRotate() *
                                                       (control->rotateCounter_ + 1.0F));
            control->rotateSwitch_ = control->doRotate_;
            object->setRotate(control->rotateSwitch_);
        } else if (!control->switcherRotate_) {
            control->doRotate_ = 0;
            control->rotateCounter_ = 0;
            object->setRotate(control->rotate_);
        }

        if (control->switcherTranslate_) {
            control->movement_ = true;
            control->doTranslate_ = control->doTranslate_ + (control->signChangeTranslate() *
                                                             (control->translateCounter_ + 1.0F));
            control->translateSwitch_ = control->translateX_ + control ->doTranslate_;
            object->setTranslate(control->translateSwitch_, control->translateY_);
        } else if (!control->switcherTranslate_) {
            if (control->movement_) {
                control->translateX_ = control->translateSwitch_;
                control->translateSwitch_ = 0;
                control->doTranslate_ = 0;
                control->movement_ = false;
            }
            object->setTranslate(control->translateX_, control->translateY_);
        }

        updateTimeExp -= 1000 / UpdatePerSecond;
        PrevUpdateTime = time;
    }

    const auto frameTimeExp = time - PrevFrameTime;
    if (static_cast<float>(frameTimeExp) > (1000 / FramesPerSecond)) {
        glutPostRedisplay();
        PrevFrameTime = time;
    }
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
    glutCreateWindow("Object Control");
    glutDisplayFunc(DisplayFunc);
    glutIdleFunc(IdleFunc);
    glutReshapeFunc(ReshapeFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void KeyboardFunc(unsigned char key, int  /*x*/, int  /*y*/) {
    switch (key) {
            /* num 8 (up) */
        case 56:
            control->translateY_ += control->stepUp_;
            control->printInfo();
            break;

            /* num 2 (down) */
        case 50:
            control->translateY_ -= control->stepDown_;
            control->printInfo();
            break;

            /* num 4 (left) */
        case 52:
            control->translateX_ -= control->stepLeft_;
            control->printInfo();
            break;

            /* num 6 (right) */
        case 54:
            control->translateX_ += control->stepRight_;
            control->printInfo();
            break;

            /* num 7 (up left) */
        case 55:
            control->translateY_ += control->stepUp_;
            control->translateX_ -= control->stepLeft_;
            control->printInfo();
            break;

            /* num 9 (up right) */
        case 57:
            control->translateY_ += control->stepUp_;
            control->translateX_ += control->stepRight_;
            control->printInfo();
            break;

            /* num 1 (down left) */
        case 49:
            control->translateY_ -= control->stepDown_;
            control->translateX_ -= control->stepLeft_;
            control->printInfo();
            break;

            /* num 3 (down right) */
        case 51:
            control->translateY_ -= control->stepDown_;
            control->translateX_ += control->stepRight_;
            control->printInfo();
            break;

            /* num 0 (reset) */
        case 48:
            control->clearAll();
            control->printInfo();
            break;

            /* num 5 (random) */
        case 53:
            control->translateX_ = static_cast<float>(control->randomPosition());
            control->translateY_ = static_cast<float>(control->randomPosition());
            control->rotate_ = static_cast<float>(control->randomPosition());
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
            ExitPrompt = true;
            break;
        case 'Y':
        case 'y':
            exit(0);
        case 'N':
        case 'n':
            ExitPrompt = false;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void SpecialFunc(int key, int  /*x*/, int  /*y*/) {
    switch (key) {
            /* up */
        case GLUT_KEY_UP:
            control->translateY_ += control->stepUp_;
            control->printInfo();
            break;

            /* down */
        case GLUT_KEY_DOWN:
            control->translateY_ -= control->stepDown_;
            control->printInfo();
            break;

            /* rotate (left) */
        case GLUT_KEY_LEFT:
            control->translateX_ -= control->stepLeft_;
            control->printInfo();
            break;

            /* rotate (right) */
        case GLUT_KEY_RIGHT:
            control->translateX_ += control->stepRight_;
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

    object = std::make_unique<Object>();
    control = std::make_unique<Control>();

    glutMainLoop();
}
