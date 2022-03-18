#include <cstdio>
#include <cstring>
#include <ctime>

#include "help.h"
#include "window.h"

const char *column1[] = {
    "Mouse",
    "left button      Camera Controls",
    "middle button    Zoom",
    "right button     Context Menu",
    nullptr
};

const char *column2[] = {
    "Keyboard",
    "f,F        Toggle Frame Rate on/off",
    "h,H        Toggle Help on/off",
    "w,W        Toggle Wireframe on/off",
    "l,L        Toggle Lighting on/off",
    "c,C        Toggle Culling on/off",
    "t,T        Toggle Texturing on/off",
    "b,B        Toggle Blending on/off",
    "k,K        Toggle Coordinate System on/off",
    "ESC        Exit",
    nullptr
};

bool  Help::_showHelp = false;
bool  Help::_showFPS = false;
bool  Help::_coordSystem = true;
int   Help::_frames = 0;
float Help::_fps = 60.0f;

void Help::toggle() {
    _showHelp = !_showHelp;
}

void Help::toggleFps() {
    _showFPS = !_showFPS;
}

void Help::toggleCoordSystem() {
    _coordSystem = !_coordSystem;
}

float Help::getFps() {
    static time_t lastTime = 0;
    static time_t startTime = 0;

    time_t now;
    time(&now);

    if (startTime == 0) {
        startTime = now;
        lastTime = now;
    }

    if (now - lastTime >= 1) {
        _fps = ((float) _frames) / (float) (now - lastTime);
        lastTime = now;
        _frames = 0;
    }

    return _fps;
}

double Help::getDelta() {
    double fps = Help::getFps();
    if (fps < 1.0) fps = 1.0;
    return 60.0 / fps;
}

void Help::drawBackground() {
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

void Help::printText(float x, float y, const char *text, void *font) {
    glRasterPos2f(x, y);
    unsigned int l = strlen(text);
    for (unsigned int i = 0; i < l; ++i)
        glutBitmapCharacter(font, text[i]);
}

void Help::printText(float x, float y, const char *text, float r, float g, float b, void *font) {
    glColor3f(r, g, b);
    printText(x, y, text, font);
}

void Help::printFps(float x, float y, void *font) {
    char fpstext[20];
    sprintf(fpstext, "FPS = %.1f", getFps());
    printText(x, y, fpstext, font);
}

void Help::draw() {
    ++_frames;
    if (!_showHelp && !_showFPS) return;

    GLfloat akt_color[4];
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glGetFloatv(GL_CURRENT_COLOR, akt_color);

    //orthogonale projektion setzen
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    if (_showHelp) {
        // hintergrund
        drawBackground();
        // title
        printText(-0.2f, 0.7f, TITLE, 1.0f, 1.0f, 0.0f, GLUT_BITMAP_HELVETICA_18);
        // tasten
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        float posy = 0.5f;
        int i = 0;
        while (column1[i]) {
            printText(-0.6f, posy, column1[i], GLUT_BITMAP_9_BY_15);
            posy -= 0.1f;
            ++i;
        }
        posy = 0.5f;
        i = 0;
        while (column2[i]) {
            printText(0.05f, posy, column2[i], GLUT_BITMAP_9_BY_15);
            posy -= 0.1f;
            ++i;
        }
    }

    if (_showFPS) {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        printFps(-0.78f, -0.78f);
    }

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glColor4fv(akt_color);
    glPopAttrib();
}

void Help::drawCoordSystem(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax,
                           GLfloat zmin, GLfloat zmax) {
    if (Help::_coordSystem) { ;
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
