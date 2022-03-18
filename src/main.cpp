#include <cstdlib>
#include <cmath>

#include "../Include/lib.h"
#include "help.h"
#include "window.h"
#include "input.h"
#include "light.h"
#include "assets.h"
#include "car.h"

Car car;

void drawScene() {
    glPolygonMode(GL_FRONT_AND_BACK, GlobalState::wireframe ? GL_LINE : GL_FILL);

    if (GlobalState::culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else glDisable(GL_CULL_FACE);

    if (GlobalState::lighting) {
        float m_amb[4] = {0.2, 0.2, 0.2, 1.0};
        float m_diff[4] = {0.2, 0.2, 0.6, 1.0};
        float m_spec[4] = {0.8, 0.8, 0.8, 1.0};
        float m_shine = 32.0;
        float m_emiss[4] = {0.0, 0.0, 0.0, 1.0};

        setMaterial(GL_FRONT_AND_BACK, m_amb, m_diff, m_spec, m_shine, m_emiss);
        setLights();
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }

    int acceleration = 0;
    int steering = 0;

    if (Key::specialKeyState(GLUT_KEY_UP)) acceleration++;
    if (Key::specialKeyState(GLUT_KEY_DOWN)) acceleration--;

    if (Key::specialKeyState(GLUT_KEY_LEFT)) steering++;
    if (Key::specialKeyState(GLUT_KEY_RIGHT)) steering--;

    car.update(static_cast<float>(acceleration), static_cast<float>(steering));
    car.draw();
}

void setCamera() {
    double x, y, z, The, Phi;
    static double radius = 10;

    if (Mouse::buttonState(GLUT_LEFT_BUTTON)) {
        GlobalState::cameraHelper[0] += Mouse::moveX();
        GlobalState::cameraHelper[1] += Mouse::moveY();
    }
    if (Mouse::buttonState(GLUT_MIDDLE_BUTTON)) {
        radius += 0.1 * Mouse::moveY();
        if (radius < 1.0) radius = 1.0;
    }

    Phi = 0.2 * GlobalState::cameraHelper[0] / GlobalState::screenSize[0] * M_PI + M_PI * 0.5;
    The = 0.2 * GlobalState::cameraHelper[1] / GlobalState::screenSize[1] * M_PI;
    x = radius * cos(Phi) * cos(The);
    y = radius * sin(The);
    z = radius * sin(Phi) * cos(The);
    GlobalState::cameraPos[0] = x;
    GlobalState::cameraPos[1] = z;
    int Oben = (The <= 0.5 * M_PI || The > 1.5 * M_PI) * 2 - 1;

    gluLookAt(x, y, z, 0, 0, 0, 0, Oben, 0);
}

void displayFunc() {
    if (Key::keyState(27)) {
        exit(0);
    }
    if (1 == Key::keyState('f') || 1 == Key::keyState('F')) {
        Help::toggleFps();
    }
    if (1 == Key::keyState('h') || 1 == Key::keyState('H')) {
        Help::toggle();
    }
    if (1 == Key::keyState('k') || 1 == Key::keyState('K')) {
        Help::toggleCoordSystem();
    }
    if (1 == Key::keyState('w') || 1 == Key::keyState('W')) {
        GlobalState::wireframe = !GlobalState::wireframe;
    }
    if (1 == Key::keyState('l') || 1 == Key::keyState('L')) {
        GlobalState::lighting = !GlobalState::lighting;
    }
    if (1 == Key::keyState('c') || 1 == Key::keyState('C')) {
        GlobalState::culling = !GlobalState::culling;
    }
    if (1 == Key::keyState('t') || 1 == Key::keyState('T')) {
        GlobalState::texturing = !GlobalState::texturing;
    }
    if (1 == Key::keyState('b') || 1 == Key::keyState('B')) {
        GlobalState::blending = !GlobalState::blending;
    }
    if (1 == Key::keyState('i') || 1 == Key::keyState('I')) {
        GlobalState::cameraHelper[0] = 0;
        GlobalState::cameraHelper[1] = 0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setCamera();

    Help::drawCoordSystem(-8, 10, -8, 10, -8, 10);


    drawScene();
    Help::draw();

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    init(argc, argv);

    loadObjects();
    loadTextures();

    car = Car(&objects["car.obj"]);

    glutMainLoop();

    return 0;
}
