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
int GlobalState::useVBOs = 0;

void init(int argc, char **argv) {
    int MainWin;        // Identifier vom Hauptfenster
    int MainMenu;        // Identifier vom Hauptmenu

    // GLUT initialisieren
    glutInit(&argc, argv);

    // Fensterinitialisierung
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitWindowPosition(WIN_POS_X, WIN_POS_Y);
    glutInitDisplayMode(USED_MODUS);
    MainWin = glutCreateWindow(PROG_NAME);

    // GLEW initialisieren
    GLenum res = glewInit();
    if (GLEW_OK != res) {
        //fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
        int i = 1;
    }

    // OpenGL Initialisierung
    glEnable(GL_DEPTH_TEST);    // Z-Buffer aktivieren

    // Menue erzeugen
    MainMenu = glutCreateMenu(menuFunc);
    glutAddMenuEntry(MENU_TEXT_WIREFRAME, ID_MENU_WIREFRAME);
    glutAddMenuEntry(MENU_TEXT_SHADE, ID_MENU_SHADE);
    glutAddMenuEntry(MENU_TEXT_NO_NORMALS, ID_MENU_NO_NORMALS);
    glutAddMenuEntry(MENU_TEXT_PER_SURFACE_NORMALS, ID_MENU_PER_SURFACE_NORMALS);
    glutAddMenuEntry(MENU_TEXT_PER_VERTEX_NORMALS, ID_MENU_PER_VERTEX_NORMALS);
    glutAddMenuEntry(MENU_TEXT_EXIT, ID_MENU_EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);        // Menue haengt an der rechten Maustaste

    // Callbackfunktionen binden
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


/////////////////////////////////////////////////////////////////////////////////
//	CALLBACK Funktion fuer Idle: wird aufgerufen, Standard-Callback
/////////////////////////////////////////////////////////////////////////////////
void idleFunc() {
    glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////
//	Resize CALLBACK Funktion - wird aufgerufen, wenn sich die
//	Fenstergroesse aendert
//		w,h: neue Breite und Hoehe des Fensters
/////////////////////////////////////////////////////////////////////////////////
void reshapeFunc(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double) w / (double) h, 1, 2000);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);    // sicherheitshalber
    glLoadIdentity();                // Modelierungsmatrix einstellen
    GlobalState::screenSize[0] = (double) w;
    GlobalState::screenSize[1] = (double) h;
}


/////////////////////////////////////////////////////////////////////////////////
//	Menu CALLBACK Funktion
/////////////////////////////////////////////////////////////////////////////////
void menuFunc(int Item) {
    switch (Item) {
        case ID_MENU_EXIT:
            exit(0);
            break;
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
    }
}
