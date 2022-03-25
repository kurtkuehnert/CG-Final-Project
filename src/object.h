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
    Mesh(const mesh_t &mesh, const attrib_t &attributes, const material_t *material);

    ~Mesh();

    void draw() const;

    void free();

private:
    GLfloat _ambient[4]{};
    GLfloat _diffuse[4]{};
    GLfloat _specular[4]{};
    GLfloat _emission[4]{};
    GLfloat _shininess;

    string _texture;

    GLuint _vbo{};
    GLsizei _vertex_count;

    GLfloat *_normal_offset;
    GLfloat *_texcoord_offset;

    bool _has_normals;
    bool _has_texcoords;
};

class Object {
public:
    ~Object();

    void load(const string &filename);

    void draw(const string &mesh_name);

    void drawAll() const;

    void free();

private:
    map<string, Mesh *> _meshes;
};

