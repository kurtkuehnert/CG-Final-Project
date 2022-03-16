#include <cstdlib>
#include <cmath>

#include "Include/gl.h"
#include "help.h"
#include "window.h"
#include "input.h"
#include "light.h"
#include "planetSimulation.h"
#include "assets.h"

PlanetSimulation *planetSimulation;

void setCamera() {
    double x, y, z, The, Phi;
    static double radius = 10;

    if (cg_mouse::buttonState(GLUT_LEFT_BUTTON)) {
        GlobalState::cameraHelper[0] += cg_mouse::moveX();
        GlobalState::cameraHelper[1] += cg_mouse::moveY();
    }
    if (cg_mouse::buttonState(GLUT_MIDDLE_BUTTON)) {
        radius += 0.1 * cg_mouse::moveY();
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
    planetSimulation->render();
}

void displayFunc() {
    if (cg_key::keyState(27)) {
        exit(0); // Escape -> Programm beenden
    } else if (1 == cg_key::keyState('f') || 1 == cg_key::keyState('F')) {
        cg_help::toggleFps();    // Framecounter on/off
    } else if (1 == cg_key::keyState('h') || 1 == cg_key::keyState('H') || 1 == cg_key::specialKeyState(GLUT_KEY_F1)) {
        cg_help::toggle();    // Hilfetext on/off
    } else if (1 == cg_key::keyState('k') || 1 == cg_key::keyState('K')) {
        cg_help::toggleKoordsystem();    // Koordinatensystem on/off
    } else if (1 == cg_key::keyState('w') || 1 == cg_key::keyState('W')) {
        GlobalState::drawMode = (GlobalState::drawMode == GL_FILL) ? GL_LINE : GL_FILL; // Wireframe on/off
    } else if (1 == cg_key::keyState('l') || 1 == cg_key::keyState('L')) {
        GlobalState::lightMode = !GlobalState::lightMode;    // Beleuchtung on/off
    } else if (1 == cg_key::keyState('i') || 1 == cg_key::keyState('I')) {
        GlobalState::cameraHelper[0] = 0;    // Initialisierung der Kamera
        GlobalState::cameraHelper[1] = 0;
    } else if (1 == cg_key::keyState('t') || 1 == cg_key::keyState('T')) {
        GlobalState::textureMode = !GlobalState::textureMode; // Texturierung on/off
    } else if (1 == cg_key::keyState('b') || 1 == cg_key::keyState('B')) {
        GlobalState::blendMode = !GlobalState::blendMode; // Blending on/off
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setCamera();

    glPolygonMode(GL_FRONT_AND_BACK, GlobalState::drawMode);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

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
        glDisable(GL_LIGHTING);
        glColor4f(0.2, 0.2, 0.6, 1.0);
    }

    glEnable(GL_NORMALIZE);

    cg_help::drawKoordsystem(-8, 10, -8, 10, -8, 10);
    drawScene();
    cg_help::draw();

    // Wireframe deaktivieren
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    glFlush();                // Daten an Server (fuer die Darstellung) schicken
    glutSwapBuffers();        // Buffers wechseln
}

int main(int argc, char **argv) {
    init(argc, argv);
    loadObjects();
    loadTextures();

    PlanetSimulation test = PlanetSimulation(objects);

    planetSimulation = &test;

    glutMainLoop();

    return 0;
}
