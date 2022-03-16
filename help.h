#pragma once

#include "Include/lib.h"

extern const char *column1[];
extern const char *column2[];

class Help {
public:
    static void toggle();

    static void toggleFps();

    static void toggleCoordSystem();

    static float getFps();

    static double getDelta();

    static void draw();

    static void drawCoordSystem(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat zmin, GLfloat zmax);

private:
    static bool _showHelp;
    static bool _showFPS;
    static bool _coordSystem;
    static int _frames;
    static float _fps;
    static float _shadow;

    static void drawBackground();

    static void printText(float x, float y, const char *text, void *font = GLUT_BITMAP_HELVETICA_18);

    static void
    printText(float x, float y, const char *text, float r, float g, float b, void *font = GLUT_BITMAP_HELVETICA_18);

    static void printTextShadow(float x, float y, const char *text, float r, float g, float b,
                                void *font = GLUT_BITMAP_HELVETICA_18);

    static void printFps(float x, float y, void *font = GLUT_BITMAP_HELVETICA_18);
};
