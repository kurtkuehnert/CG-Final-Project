#pragma once

#include <string>
#include "../Include/lib.h"

class Texture {
public:
    ~Texture();

    void load(const string &filename, bool generateMipmaps = false);

    void bind() const;

    void free();

    GLint _minFilter = GL_NEAREST;
    GLint _magFilter = GL_NEAREST;
    GLint _wrapMode = GL_REPEAT;
    GLint _envMode = GL_REPLACE;

private:
    GLuint _texture = 0;
};
