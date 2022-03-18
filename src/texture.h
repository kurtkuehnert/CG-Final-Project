#pragma once

#include <string>
#include "../Include/lib.h"

class Texture {
public:
    ~Texture();

    void load(const string &filename, bool generateMipmaps = false);

    void bind() const;

    void free();

    GLint _minFilter = GL_LINEAR;
    GLint _magFilter = GL_LINEAR;
    GLint _wrapMode = GL_REPEAT;
    GLint _envMode = GL_MODULATE;

private:
    GLuint _texture = 0;
};
