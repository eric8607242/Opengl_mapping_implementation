#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "utils/tiny_obj_loader.h"
#include <iostream>
#include <string>
class StaticMesh {
public:
    StaticMesh(const StaticMesh &rhs)=default;
    void release();

    static StaticMesh LoadMesh(const std::string &filename, bool flat_shading);
    void draw(bool flat_shading);
    void computeFlatNormal();

	bool hasNormal() const;
	bool hasUV() const;

    bool operator!=(const StaticMesh &rhs) const
    { return vao != rhs.vao; }

    StaticMesh();
    GLuint vao;
    GLuint vbo[3];
    GLuint ibo;
    GLuint numIndices;

    std::vector<tinyobj::shape_t> shapes;

    std::vector<GLfloat> smooth_position;
    std::vector<GLfloat> smooth_uv;
    std::vector<GLfloat> smooth_n;
    std::vector<GLuint> smooth_i;

    std::vector<GLfloat> positions_flat;
    std::vector<GLfloat> normals_flat;
    std::vector<GLfloat> texcoords_flat;

	bool m_uv, m_normal;
};

#endif
