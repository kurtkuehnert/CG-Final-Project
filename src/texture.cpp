#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../Include/stb_image.h"

Texture::~Texture() {
    free();
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapMode);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, _envMode);
}

void Texture::load(const string &filename, bool generateMipmaps) {
    stbi_set_flip_vertically_on_load(1);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

    glGenTextures(1, &_texture);
    bind();

    GLint format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

    if (generateMipmaps) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::free() {
    if (_texture != 0) glDeleteTextures(1, &_texture);

    _texture = 0;
    _minFilter = GL_LINEAR;
    _magFilter = GL_LINEAR;
    _wrapMode = GL_REPEAT;
    _envMode = GL_MODULATE;
}
