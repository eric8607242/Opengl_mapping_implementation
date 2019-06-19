#include "StaticMesh.h"


StaticMesh::StaticMesh()
    : m_uv(false), m_normal(false)
{

}

StaticMesh StaticMesh::LoadMesh(const std::string &filename, bool flat_shading)
{

    std::vector<tinyobj::shape_t> shapes;
	{
		std::vector<tinyobj::material_t> materials;
		std::string error_string;
		if (!tinyobj::LoadObj(shapes, materials, error_string, filename.c_str())) {
			// GG
		}

        /*
		if (shapes.size() == 0)
			GG

		if (shapes[0].mesh.texcoords.size() == 0 || shapes[0].mesh.normals.size() == 0)
			GG*/
	}


    StaticMesh ret;
    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    ret.shapes = shapes;

    
    if (flat_shading) {
        ret.computeFlatNormal();

        // Upload position array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ret.positions_flat.size(),
            ret.positions_flat.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        if (ret.texcoords_flat.size() > 0) {

            // Upload texCoord array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ret.texcoords_flat.size(),
                ret.texcoords_flat.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_uv = true;
        }

        if (ret.normals_flat.size() > 0) {
            // Upload normal array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ret.normals_flat.size(),
                ret.normals_flat.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_normal = true;
        }

        glBindVertexArray(0);      

    } else {
        ret.smooth_position = shapes[0].mesh.positions;
        ret.smooth_uv = shapes[0].mesh.texcoords;
        ret.smooth_n = shapes[0].mesh.normals;
        ret.smooth_i = shapes[0].mesh.indices;

        // Upload position array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
            shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        if (shapes[0].mesh.texcoords.size() > 0) {

            // Upload texCoord array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
                shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_uv = true;
        }

        if (shapes[0].mesh.normals.size() > 0) {

            // Upload normal array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
                shapes[0].mesh.normals.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_normal = true;
        }

        // Setup index buffer for glDrawElements
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
            shapes[0].mesh.indices.data(), GL_STATIC_DRAW);
        ret.numIndices = shapes[0].mesh.indices.size();

        glBindVertexArray(0);       
    }

    return ret;
}

void StaticMesh::release()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ibo);

}

void StaticMesh::draw(bool flat_shading)
{
    glBindVertexArray(vao);
    if (!flat_shading) {
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, numIndices);
    }
}


void StaticMesh::computeFlatNormal() {
    glm::vec3 v1;
    glm::vec3 v2;

    /* 
        compute face normals by cross-product 
        (also fill in new postions (positions_flat), 
        and new normals (normals_flat),
        and new texture coordinates (texcoords_flat))
    */
    for (int i = 0; i < shapes[0].mesh.indices.size(); i+=3) {
        v1.x = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i+1] * 3] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3];
        v1.y = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i+1] * 3 + 1] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 1];
        v1.z = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i+1] * 3 + 2] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 2];
        
        v2.x = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i+2] * 3] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3];
        v2.y = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i+2] * 3 + 1] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 1];
        v2.z = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i+2] * 3 + 2] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 2];
        

        for (int i = 0; i < 3; i++) {
            normals_flat.push_back(glm::normalize(glm::cross(v1, v2)).x);
            normals_flat.push_back(glm::normalize(glm::cross(v1, v2)).y);
            normals_flat.push_back(glm::normalize(glm::cross(v1, v2)).z);
        }
        
        // push new vertex positions
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 1]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 2]);

        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i+1] * 3]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i+1] * 3 + 1]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i+1] * 3 + 2]);

        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i+2] * 3]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i+2] * 3 + 1]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i+2] * 3 + 2]);

        // push new texture coordinates
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i] * 2]);
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i] * 2 + 1]);

        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i+1] * 2]);
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i+1] * 2 + 1]);

        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i+2] * 2]);
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i+2] * 2 + 1]);
    }

}

bool StaticMesh::hasNormal() const
{
	return m_normal;
}

bool StaticMesh::hasUV() const 
{
	return m_uv;
}


