#include "input.h"

char Key::keys[] = {0};
char Key::specialKeys[] = {0};
char Mouse::buttons[] = {0};
int Mouse::pos[] = {0};

/////////////////////////////////////////////////////////////////////////////////
//	Tastatur CALLBACK Funktion (Taste gedr�ckt)
//		key: Wert der Taste die gedrueckt wurde
//		x,y: Position des Mauskursors auf dem Viewport
/////////////////////////////////////////////////////////////////////////////////
void KeyboardDownFunc(unsigned char key, int x, int y) {
    Key::keys[key] = 1;
}


/////////////////////////////////////////////////////////////////////////////////
//	Tastatur CALLBACK Funktion (Taste losgelassen)
//		key: Wert der Taste die losgelassen wurde
//		x,y: Position des Mauskursors auf dem Viewport
/////////////////////////////////////////////////////////////////////////////////
void KeyboardUpFunc(unsigned char key, int x, int y) {
    Key::keys[key] = 0;
}


/////////////////////////////////////////////////////////////////////////////////
//	Tastatur CALLBACK Funktion
//		key: Wert der Taste die gedrueckt wurde
//		x,y: Position des Mauskursors auf dem Viewport
/////////////////////////////////////////////////////////////////////////////////
void SpecialKeyboardFunc(int key, int x, int y) {
    Key::specialKeys[key > 99 ? key - 87 : key] = 1;
}


/////////////////////////////////////////////////////////////////////////////////
//	Tastatur CALLBACK Funktion
//		key: Wert der Taste die losgelassen wurde
//		x,y: Position des Mauskursors auf dem Viewport
/////////////////////////////////////////////////////////////////////////////////
void SpecialKeyboardUpFunc(int key, int x, int y) {
    Key::specialKeys[key > 99 ? key - 87 : key] = 0;
}


/////////////////////////////////////////////////////////////////////////////////
//	Maus Button CALLBACK Funktion
//		button - Welche Taste betaetigt bzw. losgelassen wurde
//		state  - Status der State (GLUT_DOWN, GLUT_UP)
//		x, y   - Fensterkoordinaten des Mauszeigers
/////////////////////////////////////////////////////////////////////////////////
void MouseFunc(int button, int state, int x, int y) {
    Mouse::pos[2] = Mouse::pos[0];
    Mouse::pos[3] = Mouse::pos[1];
    Mouse::pos[0] = x;
    Mouse::pos[1] = y;
    if (button > -1 && button < 3) {
        Mouse::buttons[button] = char(GLUT_DOWN == state);
    }
    glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////
//	Maus Movement CALLBACK Funktion fuer GlutMotionFunc und GlutPassiveMotionFunc
/////////////////////////////////////////////////////////////////////////////////
void MouseMove(int x, int y) {
    Mouse::pos[2] = Mouse::pos[0];
    Mouse::pos[3] = Mouse::pos[1];
    Mouse::pos[0] = x;
    Mouse::pos[1] = y;
    glutPostRedisplay();
}


char Key::keyState(unsigned char k) {
    if (1 == keys[k]) {
        keys[k] = 2;
        return 1;
    }
    return keys[k];
}


char Key::specialKeyState(int k) {
    if (k > 99) k -= 87;
    if (1 == specialKeys[k]) {
        specialKeys[k] = 2;
        return 1;
    }
    return specialKeys[k];
}


char Mouse::buttonState(unsigned char b) {
    if (1 == buttons[b]) {
        buttons[b] = 2;
        return 1;
    }
    return buttons[b];
}


int Mouse::moveX() {
    return (pos[0] - pos[2]);
}


int Mouse::moveY() {
    return (pos[1] - pos[3]);
}


int Mouse::posX() {
    return pos[0];
}


int Mouse::posY() {
    return pos[1];
}
