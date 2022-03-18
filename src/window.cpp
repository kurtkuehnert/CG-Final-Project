#include "window.h"
#include "input.h"

double GlobalState::screenSize[] = {0};
bool GlobalState::wireframe = false;
bool GlobalState::lighting = true;
bool GlobalState::culling = false;
bool GlobalState::texturing = true;
bool GlobalState::blending = true;
int GlobalState::cameraHelper[] = {0};
float GlobalState::cameraPos[] = {0, 0};

void init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitWindowPosition(WIN_POS_X, WIN_POS_Y);
    glutInitDisplayMode(DISPLAY_MODE);
    glutCreateWindow(TITLE);
    glewInit();

    glEnable(GL_DEPTH_TEST);

    glutCreateMenu(menuFunc);
    glutAddMenuEntry(MENU_TEXT_WIREFRAME, MENU_WIREFRAME);
    glutAddMenuEntry(MENU_TEXT_LIGHTING, MENU_LIGHTING);
    glutAddMenuEntry(MENU_TEXT_CULLING, MENU_CULLING);
    glutAddMenuEntry(MENU_TEXT_TEXTURING, MENU_TEXTURING);
    glutAddMenuEntry(MENU_TEXT_BLENDING, MENU_BLENDING);
    glutAddMenuEntry(MENU_TEXT_EXIT, MENU_EXIT);
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
        case MENU_EXIT:
            exit(0);
        case MENU_WIREFRAME:
            GlobalState::wireframe = !GlobalState::wireframe;
            break;
        case MENU_LIGHTING:
            GlobalState::lighting = !GlobalState::lighting;
            break;
        case MENU_CULLING:
            GlobalState::culling = !GlobalState::culling;
            break;
        case MENU_TEXTURING:
            GlobalState::texturing = !GlobalState::texturing;
            break;
        case MENU_BLENDING:
            GlobalState::blending = !GlobalState::blending;
            break;
        default:
            break;
    }
}
