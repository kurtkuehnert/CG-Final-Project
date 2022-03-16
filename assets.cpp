#include "assets.h"

Image textures[NUM_TEXTURES];
cg_object3D objects[NUM_OBJECTS];

const char *texture_files[NUM_TEXTURES] = {
    "container.bmp",
    "opengl.bmp",
    "spaceship_red.png"
};

const char *objects_files[NUM_OBJECTS] = {
    "chrome.obj",
    "top.obj",
    "glass.obj",
    "wheel_single.obj",
    "wheel_double.obj",
    "wheel_screws.obj",
    "axis.obj",
    "trailer.obj",
    "cargo_t.obj",
    "ground_concrete.obj",
    "ground_street.obj",
    "spaceship.obj"
};


void loadTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        char file[255] = "../textures/";
        strcat(file, texture_files[i]);
        textures[i].load(file);
    }
}

void loadObjects() {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        char file[255] = "../objects/";
        strcat(file, objects_files[i]);
        objects[i].load(file, true);
    }

    objects[TRUCK_CHROME].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 120.0, 0.0);
    objects[TRUCK_TOP].setMaterial(0.9, 0.2, 0.3, 1.0, 0.5, 90.0, 0.0);
    objects[TRUCK_GLASS].setMaterial(0.5, 0.5, 0.9, 1.0, 0.5, 120.0, 0.0);
    objects[WHEEL_SINGLE].setMaterial(0.2, 0.2, 0.2, 1.0, 0.0, 20.0, 0.0);
    objects[WHEEL_DOUBLE].setMaterial(0.2, 0.2, 0.2, 1.0, 0.0, 20.0, 0.0);
    objects[WHEEL_SCREWS].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 20.0, 0.0);
    objects[AXIS].setMaterial(0.9, 0.3, 0.3, 1.0, 0.5, 80.0, 1.0);
    objects[TRAILER_CHASSIS].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 128.0, 0.0);
    objects[TRAILER_CARGO].setMaterial(0.8, 0.8, 0.3, 1.0, 0.0, 64.0, 0.0);
    objects[GROUND_OBJ1].setMaterial(0.4, 0.4, 0.4, 1.0, 0.0, 128.0, 0.0);
    objects[GROUND_OBJ2].setMaterial(0.3, 0.3, 0.3, 1.0, 0.0, 128.0, 0.0);
    objects[SPACESHIP].setMaterial(0.8, 0.8, 0.8, 1.0, 1.0, 120.0, 0.0);
}