#include "window.h"
#include "input.h"

double GlobalState::screenSize[] = {0};
GLenum GlobalState::drawMode = GL_FILL;
bool GlobalState::lightMode = true;
bool GlobalState::textureMode = true;
bool GlobalState::blendMode = true;
int GlobalState::normalMode = 2;
bool GlobalState::cullMode = false;
int GlobalState::cameraHelper[] = {0};
float GlobalState::cameraPos[] = {0, 0};

void init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitWindowPosition(WIN_POS_X, WIN_POS_Y);
    glutInitDisplayMode(DISPLAY_MODE);
    glutCreateWindow(TITLE);
    glewInit();

    glEnable(GL_DEPTH_TEST);    // Z-Buffer aktivieren

    glutCreateMenu(menuFunc);
    glutAddMenuEntry(MENU_TEXT_WIREFRAME, ID_MENU_WIREFRAME);
    glutAddMenuEntry(MENU_TEXT_SHADE, ID_MENU_SHADE);
    glutAddMenuEntry(MENU_TEXT_NO_NORMALS, ID_MENU_NO_NORMALS);
    glutAddMenuEntry(MENU_TEXT_PER_SURFACE_NORMALS, ID_MENU_PER_SURFACE_NORMALS);
    glutAddMenuEntry(MENU_TEXT_PER_VERTEX_NORMALS, ID_MENU_PER_VERTEX_NORMALS);
    glutAddMenuEntry(MENU_TEXT_EXIT, ID_MENU_EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc);
    glutMouseFunc(MouseFunc);
    glutMotionFunc(MouseMove);
    glutPassiveMotionFunc(MouseMove);
    glutKeyboardFunc(KeyboardDownFunc);
    glutKeyboardUpFunc(KeyboardUpFunc);
    glutSpecialFunc(SpecialKeyboardFunc);
    glutSpecialUpFunc(SpecialKeyboardUpFunc);
    glutIdleFunc(idleFunc);
    glutIgnoreKeyRepeat(GL_TRUE);
}

void idleFunc() {
    glutPostRedisplay();
}

void reshapeFunc(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, static_cast<double>(width) / static_cast<double>(height), NEAR, FAR);
    glViewport(0, 0, width, height);
    GlobalState::screenSize[0] = static_cast<double>(width);
    GlobalState::screenSize[1] = static_cast<double>(height);
}

void menuFunc(int Item) {
    switch (Item) {
        case ID_MENU_EXIT:
            exit(0);
        case ID_MENU_WIREFRAME:
            if (GlobalState::drawMode == GL_FILL)
                GlobalState::drawMode = GL_LINE;
            else
                GlobalState::drawMode = GL_FILL;
            break;
        case ID_MENU_SHADE:
            if (GlobalState::lightMode == GL_TRUE)
                GlobalState::lightMode = GL_FALSE;
            else
                GlobalState::lightMode = GL_TRUE;
            break;
        case ID_MENU_NO_NORMALS:
            GlobalState::normalMode = 0;
            break;
        case ID_MENU_PER_SURFACE_NORMALS:
            GlobalState::normalMode = 1;
            break;
        case ID_MENU_PER_VERTEX_NORMALS:
            GlobalState::normalMode = 2;
            break;
        default:
            break;
    }
}
