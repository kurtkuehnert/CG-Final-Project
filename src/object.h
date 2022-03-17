#pragma once

#include <string>
#include "../Include/lib.h"

class Object {
public:
    void load(const string &filename);

    void draw() const;

    void free();

private:
    GLfloat ambient[4] = {0.1, 0.1, 0.1, 0.1};
    GLfloat diffuse[4] = {0.5, 0.1, 0.1, 0.1};
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shininess;

    GLuint vbo;
    GLsizei vertex_count;

    GLfloat *normal_offset;
    GLfloat *texcoord_offset;

    bool has_normals;
    bool has_texcoords;
};

