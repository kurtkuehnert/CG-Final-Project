#pragma once

#include <string>
#include <map>
#include "../Include/lib.h"
#include "../Include/tiny_obj_loader.h"

using namespace tinyobj;

const material_t DEFAULT_MATERIAL = {
    "Default",
    {0.2, 0.2, 0.3},
    {0.3, 0.3, 0.3},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    0,
    0,
    1,
    1,
};

class Mesh {
public:
    Mesh() {}

    Mesh(const mesh_t &mesh, const attrib_t &attributes, const material_t *material);

    void draw() const;

    void free();

private:
    GLfloat ambient[4]{};
    GLfloat diffuse[4]{};
    GLfloat specular[4]{};
    GLfloat emission[4]{};
    GLfloat shininess;

    string diffuse_texname;

    GLuint vbo{};
    GLsizei vertex_count;

    GLfloat *normal_offset;
    GLfloat *texcoord_offset;

    bool has_normals;
    bool has_texcoords;
};

class Object {
public:
    void load(const string &filename);

    void draw(const string &mesh_name);

    void drawAll() const;

    void free();

private:
    map<string, Mesh> meshes;
};

