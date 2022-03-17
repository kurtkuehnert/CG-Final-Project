#pragma once

#include <cstring>
#include "texture.h"
#include "object.h"

#define NUM_TEXTURES 1
#define NUM_OBJECTS 1

const string OBJECT_PATH = "../assets/objects/";
const string TEXTURE_PATH = "../assets/textures/";
const string MATERIAL_PATH = "../assets/materials/";

extern Texture textures[NUM_TEXTURES];
extern Object objects[NUM_OBJECTS];
extern const char *texture_files[NUM_TEXTURES];
extern const string objects_files[NUM_OBJECTS];

enum {
    SPACESHIP,
    CAR,
    HOUSE
};

enum {
    SPACESHIP_RED
};

void loadTextures();

void loadObjects();