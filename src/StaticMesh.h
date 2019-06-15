#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
class StaticMesh {
public:
    StaticMesh(const StaticMesh &rhs)=default;
    void release();

    static StaticMesh LoadMesh(const std::string &filename);
    void draw();

	bool hasNormal() const;
	bool hasUV() const;

    bool operator!=(const StaticMesh &rhs) const
    { return vao != rhs.vao; }

    StaticMesh();
    GLuint vao;
    GLuint vbo[3];
    GLuint ibo;
    GLuint numIndices;

    std::vector<GLfloat> smooth_position;
    std::vector<GLfloat> smooth_uv;
    std::vector<GLfloat> smooth_n;
    std::vector<GLuint> smooth_i;

	bool m_uv, m_normal;
};

#endif
