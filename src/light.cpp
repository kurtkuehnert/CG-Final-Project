#include "light.h"

void setMaterial(GLenum face, GLfloat amb[4], GLfloat diff[4], GLfloat spec[4], GLfloat shine, GLfloat emis[4]) {
    glMaterialfv(face, GL_AMBIENT, amb);
    glMaterialfv(face, GL_DIFFUSE, diff);
    glMaterialfv(face, GL_SPECULAR, spec);
    glMaterialf(face, GL_SHININESS, shine);
    glMaterialfv(face, GL_EMISSION, emis);
    glColor4fv(diff);
}

void setLights() {
    glEnable(GL_LIGHTING);

    // Parameter eines globalen Lichts
    GLfloat g_amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GLU_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_amb);

    // Licht 0

    // Paramaters fuer Lichtquelle 0
    GLfloat l_pos[4] = {1.00f, 10.0f, 10.0f, 1.0f};
    GLfloat l_amb[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat l_diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat l_spec[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat l_spotdir[3] = {0.0f, 0.0f, 1.0f};
    GLfloat l_spotcutoff = 180.0f;
    GLfloat l_spotexp = 0.0f;
    GLfloat l_att[3] = {1.0f, 0.0f, 0.0f};

    // Position
    glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
    // Farbe
    glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l_spec);
    // Spot
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l_spotdir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, l_spotcutoff);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, l_spotexp);
    // Abschwaechung
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, l_att[0]);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, l_att[1]);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, l_att[2]);
    // Aktivieren
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);
}

// Standardeinstellung f�r Farbe und Beleuchtung
void setDefaultLightAndMaterial(GLboolean lightMode) {
    GLfloat color[4] = {0.4, 0.4, 0.8, 1.0};
    if (lightMode == GL_TRUE) // Beleuchtung aktivieren
    {
        GLfloat m_amb[4] = {0.2, 0.2, 0.2, 1.0};
        GLfloat *m_diff = color;
        GLfloat m_spec[4] = {0.8, 0.8, 0.8, 1.0};
        GLfloat m_shine = 32.0;
        GLfloat m_emiss[4] = {0.0, 0.0, 0.0, 1.0};

        setMaterial(GL_FRONT_AND_BACK, m_amb, m_diff, m_spec, m_shine, m_emiss);

        setLights();

        glEnable(GL_NORMALIZE);

        glEnable(GL_LIGHTING);
    } else   // Zeichnen im Farbmodus
    {
        glDisable(GL_LIGHTING);
        glColor4fv(color);
    }
}

// Abstraktion der OpenGL Lichtquellen
Light::Light(int num) {
    this->id = GL_LIGHT0 + num;
    setPosition(0.0f, 0.0f, 1.0f, 0.0f);
    setRotation(0.0f, 0.0f);
    setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
    setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
    setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    setSpotlight(0.0f, -1.0f, 0.0f, 180.0f, 0.0f);
    setAttentuation(1.0f, 0.0f, 0.0f);
    this->disable();
}


void Light::setPosition(float x, float y, float z, float w) {
    this->pos[0] = x;
    this->pos[1] = y;
    this->pos[2] = z;
    this->pos[3] = w;
}

void Light::setRotation(float horizontal, float vertikal) {
    this->rot[0] = horizontal;
    this->rot[1] = vertikal;
}


void Light::setAmbient(float r, float g, float b, float a) {
    this->amb[0] = r;
    this->amb[1] = g;
    this->amb[2] = b;
    this->amb[3] = a;
}


void Light::setDiffuse(float r, float g, float b, float a) {
    this->diff[0] = r;
    this->diff[1] = g;
    this->diff[2] = b;
    this->diff[3] = a;
}


void Light::setSpecular(float r, float g, float b, float a) {
    this->spec[0] = r;
    this->spec[1] = g;
    this->spec[2] = b;
    this->spec[3] = a;
}


void Light::setSpotlight(float directionX, float directionY, float directionZ, float cutoff, float exponent) {
    this->spot[0] = directionX;
    this->spot[1] = directionY;
    this->spot[2] = directionZ;
    this->spot[3] = cutoff;
    this->spot[4] = exponent;
}


void Light::setAttentuation(float constant, float linear, float quadric) {
    this->att[0] = constant;
    this->att[1] = linear;
    this->att[2] = quadric;
}

void Light::draw() {
    if (this->enabled) {
        glPushMatrix();
        // rotation
        glRotatef(this->rot[0], 0, 1, 0);
        glRotatef(this->rot[1], 1, 0, 0);
        // position
        glLightfv(this->id, GL_POSITION, this->pos);
        // color
        glLightfv(this->id, GL_AMBIENT, this->amb);
        glLightfv(this->id, GL_DIFFUSE, this->diff);
        glLightfv(this->id, GL_SPECULAR, this->spec);
        // spotlight
        glLightfv(this->id, GL_SPOT_DIRECTION, this->spot);
        glLightf(this->id, GL_SPOT_CUTOFF, this->spot[3]);
        glLightf(this->id, GL_SPOT_EXPONENT, this->spot[4]);
        // attentuation
        glLightf(this->id, GL_CONSTANT_ATTENUATION, this->att[0]);
        glLightf(this->id, GL_LINEAR_ATTENUATION, this->att[1]);
        glLightf(this->id, GL_QUADRATIC_ATTENUATION, this->att[2]);
        // enable
        glEnable(this->id);
        glPopMatrix();
    } else
        glDisable(this->id);
}


void Light::enable() {
    this->enabled = true;
}


void Light::disable() {
    this->enabled = false;
}


void Light::toggle() {
    this->enabled = !this->enabled;
}

void Light::markLightPosition() {
    if (this->enabled) {
        // eine kleine Kugel an die Position der Lichtquelle zeichnen
        glPushMatrix();
        glTranslatef(this->pos[0], this->pos[1], this->pos[2]),
            glScalef(0.2, 0.2, 0.2);
        glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glColor4fv(this->diff);
        glutSolidSphere(0.5, 30, 30);
        glPopAttrib();
        glPopMatrix();
    }
}


