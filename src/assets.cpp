#include "assets.h"

Texture textures[NUM_TEXTURES];
Object objects[NUM_OBJECTS];

const char *texture_files[NUM_TEXTURES] = {
    "spaceship_red.png"
};

const string objects_files[NUM_OBJECTS] = {
    // "spaceship.obj",
    "car.obj",
    // "house.obj"
};


void loadTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        textures[i].load(TEXTURE_PATH + texture_files[i]);
    }
}

void loadObjects() {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        objects[i].load(OBJECT_PATH + objects_files[i]);
    }
}