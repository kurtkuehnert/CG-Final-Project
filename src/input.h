#pragma once

#include "../Include/lib.h"

void KeyboardDownFunc(unsigned char key, int x, int y);

void KeyboardUpFunc(unsigned char key, int x, int y);

void SpecialKeyboardFunc(int key, int x, int y);

void SpecialKeyboardUpFunc(int key, int x, int y);

void MouseFunc(int button, int state, int x, int y);

void MouseMove(int x, int y);

class Key {
public:
    static char keyState(unsigned char k);

    static char specialKeyState(int k);

private:
    friend void KeyboardDownFunc(unsigned char key, int x, int y);

    friend void KeyboardUpFunc(unsigned char key, int x, int y);

    friend void SpecialKeyboardFunc(int key, int x, int y);

    friend void SpecialKeyboardUpFunc(int key, int x, int y);

    static char _keys[256];
    static char _specialKeys[31];
};

class Mouse {
public:
    static char buttonState(unsigned char b);

    static int moveX();

    static int moveY();

    static int posX();

    static int posY();

private:
    friend void MouseFunc(int button, int state, int x, int y);

    friend void MouseMove(int x, int y);

    static char _buttons[3];
    static int _pos[4];
};
