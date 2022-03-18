#pragma once

#include <cstring>
#include "texture.h"
#include "object.h"

const string OBJECT_PATH = "../assets/objects/";
const string TEXTURE_PATH = "../assets/textures/";
const string MATERIAL_PATH = "../assets/materials/";

extern map<string, Texture> textures;
extern map<string, Object> objects;
extern const string texture_files[];
extern const string object_files[];

void loadTextures();

void loadObjects();