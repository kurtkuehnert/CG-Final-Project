#include <cstdio>
#include <chrono>

#include "menu.h"
#include "window.h"

const string column1[] = {

    "Interactions",
    "1        Observer Camera",
    "2        Third Person Camera",
    "3        First Person Camera",
    "<        Toggle Left Car Door open/close",
    ">        Toggle Right Car Door open/close",
    "up       Accelerate Forwards",
    "down     Accelerate Backwards",
    "left     Steer Left",
    "right    Steer Right",
};

const string column2[] = {
    "Mouse",
    "left    Camera Controls",
    "middle  Zoom",
    "right   Context Menu",
    "",
    "Keyboard",
    "f       Toggle Frame Rate on/off",
    "h       Toggle Help on/off",
    "k       Toggle Coordinate System on/off",
    "w       Toggle Wireframe on/off",
    "l       Toggle Lighting on/off",
    "c       Toggle Culling on/off",
    "t       Toggle Texturing on/off",
    "b       Toggle Blending on/off",
    "k       Toggle Coordinate System on/off",
    "ESC     Exit",
};

bool  Menu::_help = false;
bool  Menu::_fps = false;
bool  Menu::_coordSystem = true;
float Menu::_deltaTime = 0;

void Menu::toggleHelp() {
    _help = !_help;
}

void Menu::toggleFps() {
    _fps = !_fps;
}

void Menu::toggleCoordSystem() {
    _coordSystem = !_coordSystem;
}

void Menu::update() {
    static auto lastDeltaTime = chrono::high_resolution_clock::now();

    _deltaTime = static_cast<float>(std::chrono::duration<double, std::milli>(
        chrono::high_resolution_clock::now() - lastDeltaTime).count() / 1000.0);

    lastDeltaTime = chrono::high_resolution_clock::now();
}

float Menu::getDelta() {
    return _deltaTime;
}

void Menu::draw() {
    if (!_help && !_fps) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    if (_help) {
        drawBackground();
        glColor4f(0.8f, 0.2f, 0.2f, 1.0f);
        printText(-0.2f, 0.7f, TITLE, GLUT_BITMAP_HELVETICA_18);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        float posy = 0.5f;
        for (const auto &text: column1) {
            printText(-0.6f, posy, text, GLUT_BITMAP_9_BY_15);
            posy -= 0.06f;
        }

        posy = 0.5f;
        for (const auto &text: column2) {
            printText(0.05f, posy, text, GLUT_BITMAP_9_BY_15);
            posy -= 0.06f;
        }
    }

    if (_fps) {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        printFps(-0.78f, -0.78f);
    }

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Menu::drawCoordSystem(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax,
                           GLfloat zmin, GLfloat zmax) {
    if (Menu::_coordSystem) { ;
        GLfloat akt_color[4];
        GLint akt_mode;

        glDisable(GL_CULL_FACE);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glGetFloatv(GL_CURRENT_COLOR, akt_color);
        glDisable(GL_LIGHTING);
        glGetIntegerv(GL_MATRIX_MODE, &akt_mode);
        glMatrixMode(GL_MODELVIEW);

        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(xmin, 0, 0);
        glVertex3f(xmax, 0, 0);

        for (GLfloat i = xmin; i <= xmax; i++) {
            glVertex3f(i, -0.15, 0.0);
            glVertex3f(i, 0.15, 0.0);
        }

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, ymin, 0);
        glVertex3f(0, ymax, 0);

        for (GLfloat i = ymin; i <= ymax; i++) {
            glVertex3f(-0.15, i, 0.0);
            glVertex3f(0.15, i, 0.0);
        }

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0, 0, zmin);
        glVertex3f(0, 0, zmax);

        for (GLfloat i = zmin; i <= zmax; i++) {
            glVertex3f(-0.15, 0.0, i);
            glVertex3f(0.15, 0.0, i);
        }
        glEnd();

        GLUquadricObj *cone = gluNewQuadric();

        glPushMatrix();
        glTranslatef(xmax, 0., 0.);
        glRotatef(90., 0., 1., 0.);
        glColor3f(1.0, 0.0, 0.0);
        gluCylinder(cone, 0.5, 0., 1., 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0., ymax, 0.);
        glRotatef(-90., 1., 0., 0.);
        glColor3f(0.0, 1.0, 0.0);
        gluCylinder(cone, 0.5, 0., 1., 10, 10);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0., 0., zmax);
        glColor3f(0.0, 0.0, 1.0);
        gluCylinder(cone, 0.5, 0., 1., 10, 10);
        glPopMatrix();

        gluDeleteQuadric(cone);

        glMatrixMode(akt_mode);
        glColor4fv(akt_color);
        glPopAttrib();
    }
}

void Menu::drawBackground() {
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.3f, 0.3f, 0.3f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.8f);
    glVertex2f(-0.8f, -0.8f);
    glVertex2f(0.8f, -0.8f);
    glVertex2f(0.8f, 0.8f);
    glEnd();
    glDisable(GL_BLEND);
}

void Menu::printText(float x, float y, const string &text, void *font) {
    glRasterPos2f(x, y);

    for (auto &c: text) glutBitmapCharacter(font, c);
}

void Menu::printFps(float x, float y, void *font) {
    string text = "FPS = " + to_string(1.0f / _deltaTime);
    printText(x, y, text, font);
}
