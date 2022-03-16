#pragma once

#include <cstring>
#include "image.h"
#include "wavefront.h"

#define NUM_TEXTURES 3
#define NUM_OBJECTS 12

extern Image textures[NUM_TEXTURES];
extern cg_object3D objects[NUM_OBJECTS];
extern const char *texture_files[NUM_TEXTURES];
extern const char *objects_files[NUM_OBJECTS];

enum {
    TRUCK_CHROME,
    TRUCK_TOP,
    TRUCK_GLASS,
    WHEEL_SINGLE,
    WHEEL_DOUBLE,
    WHEEL_SCREWS,
    AXIS,
    TRAILER_CHASSIS,
    TRAILER_CARGO,
    GROUND_OBJ1,
    GROUND_OBJ2,
    SPACESHIP
};

enum {
    CONTAINER,
    OPENGL,
    SPACESHIP_RED
};

void loadTextures();

void loadObjects();