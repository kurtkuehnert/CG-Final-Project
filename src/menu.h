#pragma once

#include "../Include/lib.h"
#include <string>

extern const string column1[];
extern const string column2[];

class Menu {
public:
    static void toggleHelp();

    static void toggleFps();

    static void toggleCoordSystem();

    static void update();

    static float getDelta();

    static void draw();

    static void drawCoordSystem(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

private:
    static bool _help;
    static bool _fps;
    static bool _coordSystem;
    static float _deltaTime;

    static void drawBackground();

    static void printText(float x, float y, const string &text, void *font = GLUT_BITMAP_HELVETICA_18);

    static void printFps(float x, float y, void *font = GLUT_BITMAP_HELVETICA_18);
};
