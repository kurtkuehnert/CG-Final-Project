#pragma once

#include "Include/lib.h"

#define PROGRAM_NAME  "Solar System"
#define WIN_POS_X                 0
#define WIN_POS_Y                 0
#define WIN_WIDTH              1280
#define WIN_HEIGHT              800
#define FOV                      60
#define NEAR                      1
#define FAR                    1000

#if defined NEED_STENCIL && defined NEED_ACCUM
#define DISPLAY_MODE	GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_ACCUM
#elif defined NEED_STENCIL
#define DISPLAY_MODE	GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL
#elif defined NEED_ACCUM
#define DISPLAY_MODE	GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ACCUM
#else
#define DISPLAY_MODE    GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE
#endif

#define MENU_TEXT_WIREFRAME                "Wireframe on/off"
#define MENU_TEXT_SHADE                    "Lighting/Shading"
#define MENU_TEXT_NO_NORMALS               "Lighting: set No Normals"
#define MENU_TEXT_PER_SURFACE_NORMALS      "Lighting: set normals per surface"
#define MENU_TEXT_PER_VERTEX_NORMALS       "Lighting: set normals per vertex"
#define MENU_TEXT_EXIT                     "Exit"

enum {
    ID_MENU_WIREFRAME,
    ID_MENU_SHADE,
    ID_MENU_NO_NORMALS,
    ID_MENU_PER_SURFACE_NORMALS,
    ID_MENU_PER_VERTEX_NORMALS,
    ID_MENU_EXIT
};

void idleFunc();

void displayFunc();

void reshapeFunc(int, int);

void menuFunc(int);

void init(int argc, char **argv);

struct GlobalState {
    static double screenSize[2];
    static GLenum drawMode;
    static bool lightMode;
    static int normalMode;
    static bool cullMode;
    static bool textureMode;
    static bool blendMode;
    static int cameraHelper[2];
    static float cameraPos[2];
    static int useVBOs;
};
