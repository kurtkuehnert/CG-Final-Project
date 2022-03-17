#include <cstdlib>
#include <cmath>

#include "../Include/lib.h"
#include "help.h"
#include "window.h"
#include "input.h"
#include "light.h"
#include "assets.h"

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

void drawScene() {
    glEnable(GL_TEXTURE_2D);
    textures[SPACESHIP_RED].bind();
    objects[SPACESHIP].draw();
    glDisable(GL_TEXTURE_2D);

    //objects[CAR].draw();
    //objects[HOUSE].draw();
}

void displayFunc() {
    if (Key::keyState(27)) {
        exit(0);
    } else if (1 == Key::keyState('f') || 1 == Key::keyState('F')) {
        Help::toggleFps();
    } else if (1 == Key::keyState('h') || 1 == Key::keyState('H') || 1 == Key::specialKeyState(GLUT_KEY_F1)) {
        Help::toggle();
    } else if (1 == Key::keyState('k') || 1 == Key::keyState('K')) {
        Help::toggleCoordSystem();
    } else if (1 == Key::keyState('w') || 1 == Key::keyState('W')) {
        GlobalState::drawMode = (GlobalState::drawMode == GL_FILL) ? GL_LINE : GL_FILL;
    } else if (1 == Key::keyState('l') || 1 == Key::keyState('L')) {
        GlobalState::lightMode = !GlobalState::lightMode;
    } else if (1 == Key::keyState('i') || 1 == Key::keyState('I')) {
        GlobalState::cameraHelper[0] = 0;
        GlobalState::cameraHelper[1] = 0;
    } else if (1 == Key::keyState('t') || 1 == Key::keyState('T')) {
        GlobalState::textureMode = !GlobalState::textureMode;
    } else if (1 == Key::keyState('b') || 1 == Key::keyState('B')) {
        GlobalState::blendMode = !GlobalState::blendMode;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setCamera();

    glPolygonMode(GL_FRONT_AND_BACK, GlobalState::drawMode);
    if (GlobalState::cullMode) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    if (GlobalState::lightMode) {
        float m_amb[4] = {0.2, 0.2, 0.2, 1.0};
        float m_diff[4] = {0.2, 0.2, 0.6, 1.0};
        float m_spec[4] = {0.8, 0.8, 0.8, 1.0};
        float m_shine = 32.0;
        float m_emiss[4] = {0.0, 0.0, 0.0, 1.0};

        setMaterial(GL_FRONT_AND_BACK, m_amb, m_diff, m_spec, m_shine, m_emiss);
        setLights();
        glEnable(GL_LIGHTING);
    } else {
        glColor4f(0.2, 0.2, 0.6, 1.0);
        glDisable(GL_LIGHTING);
    }

    glEnable(GL_NORMALIZE);

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

    glutMainLoop();

    return 0;
}
