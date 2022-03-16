#pragma once

#include "Include/gl.h"

class Image {
public:
    ~Image();

    void load(const char *fileName, bool generateMipmaps = false);

    void bind() const;

    void free();

    GLint _minFilter = GL_NEAREST;
    GLint _magFilter = GL_NEAREST;
    GLint _wrapMode = GL_REPEAT;
    GLint _envMode = GL_REPLACE;

private:
    GLuint _texture = 0;
};
