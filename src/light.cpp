#include "light.h"

void setLights() {
    glEnable(GL_LIGHTING);

    GLfloat g_amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GLU_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_amb);
}

Light::Light(int num) : _id(GL_LIGHT0 + num) {}

void Light::setPosition(float x, float y, float z, float w) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = w;
}

void Light::setRotation(float horizontal, float vertikal) {
    rot[0] = horizontal;
    rot[1] = vertikal;
}


void Light::setAmbient(float r, float g, float b, float a) {
    _amb[0] = r;
    _amb[1] = g;
    _amb[2] = b;
    _amb[3] = a;
}


void Light::setDiffuse(float r, float g, float b, float a) {
    _diff[0] = r;
    _diff[1] = g;
    _diff[2] = b;
    _diff[3] = a;
}


void Light::setSpecular(float r, float g, float b, float a) {
    _spec[0] = r;
    _spec[1] = g;
    _spec[2] = b;
    _spec[3] = a;
}


void Light::setSpotlight(float directionX, float directionY, float directionZ, float cutoff, float exponent) {
    spot[0] = directionX;
    spot[1] = directionY;
    spot[2] = directionZ;
    spot[3] = cutoff;
    spot[4] = exponent;
}


void Light::setAttentuation(float constant, float linear, float quadric) {
    _att[0] = constant;
    _att[1] = linear;
    _att[2] = quadric;
}


void Light::draw() {
    if (enabled) {
        glPushMatrix();
        glRotatef(rot[0], 0, 1, 0);
        glRotatef(rot[1], 1, 0, 0);
        glLightfv(_id, GL_POSITION, pos);
        glPopMatrix();

        glLightfv(_id, GL_AMBIENT, _amb);
        glLightfv(_id, GL_DIFFUSE, _diff);
        glLightfv(_id, GL_SPECULAR, _spec);

        glLightfv(_id, GL_SPOT_DIRECTION, spot);
        glLightf(_id, GL_SPOT_CUTOFF, spot[3]);
        glLightf(_id, GL_SPOT_EXPONENT, spot[4]);

        glLightf(_id, GL_CONSTANT_ATTENUATION, _att[0]);
        glLightf(_id, GL_LINEAR_ATTENUATION, _att[1]);
        glLightf(_id, GL_QUADRATIC_ATTENUATION, _att[2]);

        glEnable(_id);
    } else
        glDisable(_id);
}

void Light::toggle() {
    enabled = !enabled;
}

void Light::markLightPosition() {
    if (enabled) {
        glPushMatrix();
        glTranslatef(pos[0], pos[1], pos[2]);
        glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glColor4fv(_diff);
        glutSolidSphere(0.2, 30, 30);
        glPopAttrib();
        glPopMatrix();
    }
}
