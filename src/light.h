#ifndef LIGHT_H
#define LIGHT_H

#ifdef _WIN32
#include "Include/glew.h"
#include "Include/freeglut.h"
#elif __linux__

#include "GL/glew.h"
#include "GL/glut.h"

#elif __APPLE__
#include "Include/glew.h"
#include "Include/freeglut.h"
#endif

// Farbe setzen fuer Farb- und Beleuchtungsmodus
void setColor(GLfloat r, GLfloat g, GLfloat b);

// Material setzen (nur fuer Beleuchtungsmodus)
void setMaterial(GLenum face, GLfloat amb[4], GLfloat diff[4], GLfloat spec[4], GLfloat shine, GLfloat emis[4]);

void setMaterial(GLenum face, float red, float green, float blue, float alpha, float specular, float shininess,
                 float emission);

// Lichtquelle(n) setzen
void setLights();

// Standardeinstellung f�r Farbe und Beleuchtung
void setDefaultLightAndMaterial(GLboolean lightMode = false);

// Abstraktion der OpenGL Lichtquellen
class Light {
public:
    // Konstructior, id zwischen 0-7 muss uebergeben werden
    Light(int num);

    // wenn w=0 -> direktionales Licht, Richtung wird eingespeichert
    // wenn w=1 -> Punktlichtquelle, Positon wird eingespeichert
    void setPosition(float x, float y, float z, float w);

    // Rotation/Strahlrichtung
    void setRotation(float horizontal, float vertikal);

    // ambienten Anteil der Lichtfarbe einspeichern
    void setAmbient(float r, float g, float b, float a);

    // diffusen Anteil der Lichtfarbe einspeichern
    void setDiffuse(float r, float g, float b, float a);

    // specularen Anteil der Lichtfarbe einspeichern
    void setSpecular(float r, float g, float b, float a);

    // Richtung, Oeffnungswinkel und exponent einspeichern (nur bei Punktlichtquelle sinnvoll)
    // cutoff = [0..90], 180
    // exponent = [0..128]
    void setSpotlight(float directionX, float directionY, float directionZ, float cutoff, float exponent);

    // Abschw�chungsfaktoren der Lichtintensit�t einspeichern
    void setAttentuation(float constant, float linear, float quadric);

    // Getter
    float *getAmbient(void) { return amb; };

    float *getDiffuse(void) { return diff; };

    float *getSpecular(void) { return spec; };

    float *getSpotDirection(void) { return spot; };

    float getSpotCutoff(void) { return spot[3]; };

    float getSpotExponent(void) { return spot[4]; };

    // Licht mit den bisher eingespeicherten Parametern rendern (falls es eingeschaltet ist)
    void draw();

    // Lichtquelle einschalten
    void enable();

    // Lichtquelle ausschalten
    void disable();

    // Lichtquelle umschalten
    void toggle();

    // eine kleine Kugel in der Farbe des diffusen Lichtanteils an die Position der Lichtquelle zeichnen
    void markLightPosition();

    float pos[4];
    float rot[2];
    float spot[5];
    bool enabled;

private:
    int id;
    float amb[4];
    float diff[4];
    float spec[4];
    float att[3];
};

#endif // LIGHT_H
