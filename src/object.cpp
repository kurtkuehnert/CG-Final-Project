#include <iostream>

#include "object.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "../Include/tiny_obj_loader.h"

#include "assets.h"
#include "window.h"

Mesh::Mesh(const mesh_t &mesh, const attrib_t &attributes, const material_t *material) {
    _vertex_count = static_cast<GLsizei>(mesh.indices.size());
    _normal_offset = reinterpret_cast<GLfloat *>(3 * _vertex_count * sizeof(GLfloat));
    _texcoord_offset = reinterpret_cast<GLfloat *>(6 * _vertex_count * sizeof(GLfloat));
    size_t vboSize = 8 * _vertex_count * sizeof(GLfloat);
    auto *vboData = static_cast<GLfloat *>(malloc(vboSize));

    GLfloat *vertices = vboData;
    GLfloat *normals = vboData + 3 * _vertex_count;
    GLfloat *texcoords = vboData + 6 * _vertex_count;

    size_t index_offset = 0;
    for (size_t face = 0; face < mesh.num_face_vertices.size(); face++) {
        auto face_vertices = size_t(mesh.num_face_vertices[face]);

        for (size_t vertex = 0; vertex < face_vertices; vertex++) {
            size_t vboIndex = index_offset + vertex;
            index_t attribute_index = mesh.indices[vboIndex];

            for (size_t i = 0; i < 3; i++) {
                size_t vertex_index = 3 * size_t(attribute_index.vertex_index) + i;
                vertices[3 * vboIndex + i] = attributes.vertices[vertex_index];
            }

            if (attribute_index.normal_index >= 0) {
                _has_normals = true;
                for (size_t i = 0; i < 3; i++) {
                    size_t normal_index = 3 * size_t(attribute_index.normal_index) + i;
                    normals[3 * vboIndex + i] = attributes.normals[normal_index];
                }
            }

            if (attribute_index.texcoord_index >= 0) {
                _has_texcoords = true;
                for (size_t i = 0; i < 2; i++) {
                    size_t texcoord_index = 2 * size_t(attribute_index.texcoord_index) + i;
                    texcoords[2 * vboIndex + i] = attributes.texcoords[texcoord_index];
                }
            }
        }

        index_offset += face_vertices;
    }

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vboSize), vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ::free(vboData);

    _ambient[0] = material->ambient[0];
    _ambient[1] = material->ambient[1];
    _ambient[2] = material->ambient[2];
    _ambient[3] = 1.0;

    _diffuse[0] = material->diffuse[0];
    _diffuse[1] = material->diffuse[1];
    _diffuse[2] = material->diffuse[2];
    _diffuse[3] = material->dissolve;

    _specular[0] = material->specular[0];
    _specular[1] = material->specular[1];
    _specular[2] = material->specular[2];
    _specular[3] = 1.0;

    _emission[0] = material->emission[0];
    _emission[1] = material->emission[1];
    _emission[2] = material->emission[2];
    _emission[3] = 1.0;

    _texture = material->diffuse_texname;

    _shininess = material->shininess;
}

Mesh::~Mesh() {
    free();
}

void Mesh::draw() const {
    glColor4fv(_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, _emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, _shininess);

    if (GlobalState::texturing && !_texture.empty()) {
        glEnable(GL_TEXTURE_2D);
        textures[_texture].bind();
    }

    if (GlobalState::blending && _diffuse[3] < 1.0f) {
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    if (_has_normals) glEnableClientState(GL_NORMAL_ARRAY);
    if (_has_texcoords) glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glVertexPointer(3, GL_FLOAT, 0, nullptr);
    if (_has_normals) glNormalPointer(GL_FLOAT, 0, _normal_offset);
    if (_has_texcoords) glTexCoordPointer(2, GL_FLOAT, 0, _texcoord_offset);

    glDrawArrays(GL_TRIANGLES, 0, _vertex_count);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (_has_normals) glDisableClientState(GL_NORMAL_ARRAY);
    if (_has_texcoords) glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Mesh::free() {
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);

    _has_normals = false;
    _has_texcoords = false;
}

Object::~Object() {
    free();
}

void Object::load(const string &filename) {
    ObjReader reader;
    ObjReaderConfig config;
    config.mtl_search_path = MATERIAL_PATH;

    if (!reader.ParseFromFile(filename, config)) {
        if (!reader.Error().empty()) {
            cerr << "TinyObjReader: " << reader.Error();
        }

        exit(1);
    }

    if (!reader.Warning().empty()) {
        cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    for (const auto &shape: shapes) {
        int material_id = shape.mesh.material_ids[0];
        const material_t *material;

        if (material_id == -1) material = &DEFAULT_MATERIAL;
        else material = &materials[material_id];

        _meshes[shape.name] = new Mesh(shape.mesh, attrib, material);
    }
}


void Object::draw(const string &mesh_name) {
    _meshes[mesh_name]->draw();
}

void Object::drawAll() const {
    for (const auto &[_, mesh]: _meshes) {
        mesh->draw();
    }
}

void Object::free() {
    for (auto &[_, mesh]: _meshes) {
        delete mesh;
    }

    _meshes.clear();
}
