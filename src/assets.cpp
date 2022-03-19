#include "assets.h"

map<string, Texture> textures;
map<string, Object> objects;

const string texture_files[] = {
    "car.png",
};

const string object_files[] = {
    "car.obj",
    // "house.obj"
};


void loadTextures() {
    for (const auto &texture_file: texture_files) {
        textures[texture_file].load(TEXTURE_PATH + texture_file);
    }
}

void loadObjects() {
    for (const auto &object_file: object_files) {
        objects[object_file].load(OBJECT_PATH + object_file);
    }
}