#include <iostream>

#include "object.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "../Include/tiny_obj_loader.h"
#include "assets.h"

using namespace tinyobj;

class Loader {
    Loader() {
        config.mtl_search_path = MATERIAL_PATH;
    }

    static Loader *instance;

public:
    ObjReaderConfig config;
    ObjReader reader;

    static Loader *getInstance() {
        if (instance == nullptr) instance = new Loader();

        return instance;
    }
};

Loader *Loader::instance = nullptr;

void Object::load(const string &filename) {
    Loader *loader = Loader::getInstance();

    if (!loader->reader.ParseFromFile(filename, loader->config)) {
        if (!loader->reader.Error().empty()) {
            cerr << "TinyObjReader: " << loader->reader.Error();
        }

        exit(1);
    }

    if (!loader->reader.Warning().empty()) {
        cout << "TinyObjReader: " << loader->reader.Warning();
    }

    auto &attrib = loader->reader.GetAttrib();
    auto &shapes = loader->reader.GetShapes();
    auto &materials = loader->reader.GetMaterials();

    size_t vboSize;
    GLfloat *vboData;

    for (const auto &shape: shapes) {
        auto mesh = shape.mesh;

        cout << shape.name << "\n";

        vertex_count = static_cast<GLsizei>(mesh.indices.size());
        normal_offset = reinterpret_cast<GLfloat *>(3 * vertex_count * sizeof(GLfloat));
        texcoord_offset = reinterpret_cast<GLfloat *>(6 * vertex_count * sizeof(GLfloat));
        vboSize = 12 * vertex_count * sizeof(GLfloat);
        vboData = static_cast<GLfloat *>(malloc(vboSize));

        cout << vertex_count << "\n";

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
                    vertices[3 * vboIndex + i] = attrib.vertices[vertex_index];
                }

                if (attribute_index.normal_index >= 0) {
                    has_normals = true;
                    for (size_t i = 0; i < 3; i++) {
                        size_t normal_index = 3 * size_t(attribute_index.normal_index) + i;
                        normals[3 * vboIndex + i] = attrib.normals[normal_index];
                    }
                }

                if (attribute_index.texcoord_index >= 0) {
                    has_texcoords = true;
                    for (size_t i = 0; i < 2; i++) {
                        size_t texcoord_index = 2 * size_t(attribute_index.texcoord_index) + i;
                        texcoords[2 * vboIndex + i] = attrib.texcoords[texcoord_index];
                    }
                }
            }

            index_offset += face_vertices;
        }

        break;
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vboSize), vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ::free(vboData);

    cout << "Loaded\n";
}


void Object::draw() const {
    glColor4fv(diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableClientState(GL_VERTEX_ARRAY);
    if (has_normals) glEnableClientState(GL_NORMAL_ARRAY);
    if (has_texcoords) glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, nullptr);
    if (has_normals) glNormalPointer(GL_FLOAT, 0, normal_offset);
    if (has_texcoords) glTexCoordPointer(2, GL_FLOAT, 0, texcoord_offset);

    glDrawArrays(GL_TRIANGLES, 0, vertex_count);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (has_normals) glDisableClientState(GL_NORMAL_ARRAY);
    if (has_texcoords) glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::free() {
    if (vbo != 0) glDeleteBuffers(1, &vbo);

    has_normals = false;
    has_texcoords = false;
}
