#include <cstdlib>

#include "../Include/lib.h"
#include "menu.h"
#include "window.h"
#include "input.h"
#include "light.h"
#include "assets.h"
#include "car.h"
#include "camera.h"

Car *car;
Light *sun;
Camera *camera;

void updateCar() {
    int acceleration = 0;
    int steering = 0;

    if (Key::specialKeyState(GLUT_KEY_UP)) acceleration++;
    if (Key::specialKeyState(GLUT_KEY_DOWN)) acceleration--;
    if (Key::specialKeyState(GLUT_KEY_LEFT)) steering++;
    if (Key::specialKeyState(GLUT_KEY_RIGHT)) steering--;
    if (1 == Key::keyState('<')) car->toggleLeftDoor();
    if (1 == Key::keyState('>')) car->toggleRightDoor();
    if (1 == Key::keyState('*')) car->toggleHeadlights();

    car->update(static_cast<float>(acceleration), static_cast<float>(steering));
}

void drawScene() {
    Menu::drawCoordSystem(-8, 10, -8, 10, -8, 10);

    if (sun->enabled) {
        glPushMatrix();
        auto position = car->getPosition();
        glTranslatef(position.x(), position.y(), position.z());
        glScalef(1000, 1000, 1000);
        glCullFace(GL_FRONT);
        objects["skybox.obj"].drawAll();
        glCullFace(GL_BACK);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.0f);
    glScalef(1000, 1, 1000);
    objects["ground.obj"].drawAll();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30.0f, 0.0f, 10.0f);
    glScalef(1.5, 1.5, 1.5);
    objects["house.obj"].drawAll();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20.0f, 0.0f, -20.0f);
    glScalef(0.3, 0.3, 0.3);
    objects["tree.obj"].drawAll();
    glPopMatrix();

    car->draw();
}

void displayFunc() {
    if (1 == Key::keyState(27)) exit(0);
    if (1 == Key::keyState('f')) Menu::toggleFps();
    if (1 == Key::keyState('h')) Menu::toggleHelp();
    if (1 == Key::keyState('k')) Menu::toggleCoordSystem();
    if (1 == Key::keyState('w')) GlobalState::wireframe = !GlobalState::wireframe;
    if (1 == Key::keyState('l')) GlobalState::lighting = !GlobalState::lighting;
    if (1 == Key::keyState('c')) GlobalState::culling = !GlobalState::culling;
    if (1 == Key::keyState('t')) GlobalState::texturing = !GlobalState::texturing;
    if (1 == Key::keyState('b')) GlobalState::blending = !GlobalState::blending;
    if (1 == Key::keyState('1')) camera->setMode(Observer);
    if (1 == Key::keyState('2')) camera->setMode(ThirdPerson);
    if (1 == Key::keyState('3')) camera->setMode(FirstPerson);
    if (1 == Key::keyState('g')) sun->toggle();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPolygonMode(GL_FRONT_AND_BACK, GlobalState::wireframe ? GL_LINE : GL_FILL);

    if (GlobalState::culling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else glDisable(GL_CULL_FACE);

    if (GlobalState::lighting) {
        setLights();
    } else glDisable(GL_LIGHTING);

    updateCar();

    camera->update(car->getPosition(), car->getRotation());

    sun->draw();
    car->lights();

    Menu::update();
    drawScene();
    Menu::draw();

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    init(argc, argv);

    loadObjects();
    loadTextures();

    camera = new Camera();

    car = new Car(&objects["car.obj"]);

    sun = new Light(0);
    sun->setPosition(-5, 10, 10, 0);

    glutMainLoop();

    delete car;
    delete sun;
    delete camera;

    return 0;
}
