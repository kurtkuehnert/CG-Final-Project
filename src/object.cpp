#include <iostream>

#include "object.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "../Include/tiny_obj_loader.h"

#include "assets.h"
#include "window.h"

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

        meshes[shape.name] = Mesh(shape.mesh, attrib, material);
    }
}


void Object::draw(const string &mesh_name) {
    meshes[mesh_name].draw();
}

void Object::free() {
    for (auto &[_, mesh]: meshes) {
        mesh.free();
    }
}

void Object::drawAll() const {
    for (const auto &[_, mesh]: meshes) {
        mesh.draw();
    }
}

Mesh::Mesh(const mesh_t &mesh, const attrib_t &attributes, const material_t *material) {
    vertex_count = static_cast<GLsizei>(mesh.indices.size());
    normal_offset = reinterpret_cast<GLfloat *>(3 * vertex_count * sizeof(GLfloat));
    texcoord_offset = reinterpret_cast<GLfloat *>(6 * vertex_count * sizeof(GLfloat));
    size_t vboSize = 8 * vertex_count * sizeof(GLfloat);
    auto *vboData = static_cast<GLfloat *>(malloc(vboSize));

    GLfloat *vertices = vboData;
    GLfloat *normals = vboData + 3 * vertex_count;
    GLfloat *texcoords = vboData + 6 * vertex_count;

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
                has_normals = true;
                for (size_t i = 0; i < 3; i++) {
                    size_t normal_index = 3 * size_t(attribute_index.normal_index) + i;
                    normals[3 * vboIndex + i] = attributes.normals[normal_index];
                }
            }

            if (attribute_index.texcoord_index >= 0) {
                has_texcoords = true;
                for (size_t i = 0; i < 2; i++) {
                    size_t texcoord_index = 2 * size_t(attribute_index.texcoord_index) + i;
                    texcoords[2 * vboIndex + i] = attributes.texcoords[texcoord_index];
                }
            }
        }

        index_offset += face_vertices;
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vboSize), vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ::free(vboData);

    ambient[0] = material->ambient[0];
    ambient[1] = material->ambient[1];
    ambient[2] = material->ambient[2];
    ambient[3] = 1.0;

    diffuse[0] = material->diffuse[0];
    diffuse[1] = material->diffuse[1];
    diffuse[2] = material->diffuse[2];
    diffuse[3] = 1.0;

    specular[0] = material->specular[0];
    specular[1] = material->specular[1];
    specular[2] = material->specular[2];
    specular[3] = 1.0;

    emission[0] = material->emission[0];
    emission[1] = material->emission[1];
    emission[2] = material->emission[2];
    emission[3] = 1.0;

    diffuse_texname = material->diffuse_texname;

    shininess = material->shininess;
}

void Mesh::draw() const {
    glColor4fv(diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


    glEnable(GL_TEXTURE_2D);
    if (!diffuse_texname.empty() && GlobalState::texturing) textures[diffuse_texname].bind();

    glEnableClientState(GL_VERTEX_ARRAY);
    if (has_normals) glEnableClientState(GL_NORMAL_ARRAY);
    if (has_texcoords) glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexPointer(3, GL_FLOAT, 0, nullptr);
    if (has_normals) glNormalPointer(GL_FLOAT, 0, normal_offset);
    if (has_texcoords) glTexCoordPointer(2, GL_FLOAT, 0, texcoord_offset);

    glDrawArrays(GL_TRIANGLES, 0, vertex_count);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (has_normals) glDisableClientState(GL_NORMAL_ARRAY);
    if (has_texcoords) glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Mesh::free() {
    if (vbo != 0) glDeleteBuffers(1, &vbo);

    has_normals = false;
    has_texcoords = false;
}
