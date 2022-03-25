#include "input.h"

char Key::_keys[] = {0};
char Key::_specialKeys[] = {0};
char Mouse::_buttons[] = {0};
int Mouse::_pos[] = {0};

void KeyboardDownFunc(unsigned char key, int x, int y) {
    Key::_keys[key] = 1;
}

void KeyboardUpFunc(unsigned char key, int x, int y) {
    Key::_keys[key] = 0;
}

void SpecialKeyboardFunc(int key, int x, int y) {
    Key::_specialKeys[key > 99 ? key - 87 : key] = 1;
}

void SpecialKeyboardUpFunc(int key, int x, int y) {
    Key::_specialKeys[key > 99 ? key - 87 : key] = 0;
}

void MouseFunc(int button, int state, int x, int y) {
    Mouse::_pos[2] = Mouse::_pos[0];
    Mouse::_pos[3] = Mouse::_pos[1];
    Mouse::_pos[0] = x;
    Mouse::_pos[1] = y;
    if (button > -1 && button < 3) {
        Mouse::_buttons[button] = char(GLUT_DOWN == state);
    }
    glutPostRedisplay();
}

void MouseMove(int x, int y) {
    Mouse::_pos[2] = Mouse::_pos[0];
    Mouse::_pos[3] = Mouse::_pos[1];
    Mouse::_pos[0] = x;
    Mouse::_pos[1] = y;
    glutPostRedisplay();
}

char Key::keyState(unsigned char k) {
    if (1 == _keys[k]) {
        _keys[k] = 2;
        return 1;
    }
    return _keys[k];
}

char Key::specialKeyState(int k) {
    if (k > 99) k -= 87;
    if (1 == _specialKeys[k]) {
        _specialKeys[k] = 2;
        return 1;
    }
    return _specialKeys[k];
}

char Mouse::buttonState(unsigned char b) {
    if (1 == _buttons[b]) {
        _buttons[b] = 2;
        return 1;
    }
    return _buttons[b];
}

int Mouse::moveX() {
    return (_pos[0] - _pos[2]);
}

int Mouse::moveY() {
    return (_pos[1] - _pos[3]);
}

int Mouse::posX() {
    return _pos[0];
}

int Mouse::posY() {
    return _pos[1];
}
