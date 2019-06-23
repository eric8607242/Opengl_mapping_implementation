#include "StaticMesh.h"

void crossProduct(float vect_A[], float vect_B[], float cross_P[])
{

    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}
void normalize(float A[], float B[])
{
    float max = A[0];
    float min = A[0];
    for (int i = 0; i < 2; i++)
    {
        if (max < A[i + 1])
        {
            max = A[i + 1];
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (min > A[i + 1])
        {
            min = A[i + 1];
        }
    }
    //printf("%f", min);
    //for (int i = 0; i < 3; i++) {
    //	B[i] = (A[i] - min) / (max - min);
    //}
    double _v_ = sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
    for (int i = 0; i < 3; i++)
    {
        B[i] = A[i] / _v_;
    }
    //printf("%f\n", _v_);
}

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
        if (!tinyobj::LoadObj(shapes, materials, error_string, filename.c_str()))
        {
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

    glGenBuffers(5, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    ret.shapes = shapes;

    int n = shapes[0].mesh.positions.size();
    //test_1 = new int[n];
    float *tgan;
    tgan = new float[n];
    float *bitgan;
    bitgan = new float[n];
    int v_n = n / 3;
    //float tgan[1536];
    //float bitgan[1536];
    //int vex_count[512];
    int *vex_count;
    vex_count = new int[v_n];

    for (int i = 0; i < shapes[0].mesh.normals.size(); i = i + 1)
    {
        tgan[i] = 0.0;
        bitgan[i] = 0.0;
    }
    for (int i = 0; i < v_n; i = i + 1)
    {
        vex_count[i] = 0;
    }
    for (int i = 0; i < shapes[0].mesh.indices.size(); i = i + 3)
    {

        //temp_indice[i] = shapes[0].mesh.indices[i];
        float edge1[3], edge2[3], deltaUV1[2], deltaUV2[2], tangent1[3], bitangent1[3], n_tangent1[3], n_bitangent1[3];
        int p1 = shapes[0].mesh.indices[i];
        int p2 = shapes[0].mesh.indices[i + 1];
        int p3 = shapes[0].mesh.indices[i + 2];
        printf("mesh: %d %d %d \n", p1, p2, p3);

        vex_count[p1] = vex_count[p1] + 1;
        vex_count[p2] = vex_count[p2] + 1;
        vex_count[p3] = vex_count[p3] + 1;
        //edge1[0] = shapes[0].mesh.positions[i * 3];
        edge1[0] = shapes[0].mesh.positions[p2 * 3] - shapes[0].mesh.positions[p1 * 3];
        edge1[1] = shapes[0].mesh.positions[p2 * 3 + 1] - shapes[0].mesh.positions[p1 * 3 + 1];
        edge1[2] = shapes[0].mesh.positions[p2 * 3 + 2] - shapes[0].mesh.positions[p1 * 3 + 2];
        edge2[0] = shapes[0].mesh.positions[p3 * 3] - shapes[0].mesh.positions[p1 * 3];
        edge2[1] = shapes[0].mesh.positions[p3 * 3 + 1] - shapes[0].mesh.positions[p1 * 3 + 1];
        edge2[2] = shapes[0].mesh.positions[p3 * 3 + 2] - shapes[0].mesh.positions[p1 * 3 + 2];
        //printf("edge1: %f %f %f \n", shapes[0].mesh.positions[p2 * 3], shapes[0].mesh.positions[p2 * 3 + 1], shapes[0].mesh.positions[p2 * 3 + 2]);

        deltaUV1[0] = shapes[0].mesh.texcoords[p2 * 2] - shapes[0].mesh.texcoords[p1 * 2];
        deltaUV1[1] = shapes[0].mesh.texcoords[p2 * 2 + 1] - shapes[0].mesh.texcoords[p1 * 2 + 1];
        deltaUV2[0] = shapes[0].mesh.texcoords[p3 * 2] - shapes[0].mesh.texcoords[p1 * 2];
        deltaUV2[1] = shapes[0].mesh.texcoords[p3 * 2 + 1] - shapes[0].mesh.texcoords[p1 * 2 + 1];
        GLfloat f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);
        tangent1[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
        tangent1[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
        tangent1[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
        //printf("t: %f %f %f \n", tangent1[0], tangent1[1], tangent1[2]);

        //normalize(tangent1,n_tangent1);
        n_tangent1[0] = tangent1[0];
        n_tangent1[1] = tangent1[1];
        n_tangent1[2] = tangent1[2];
        tgan[p1 * 3] = tgan[p1 * 3] + n_tangent1[0];
        tgan[p1 * 3 + 1] = tgan[p1 * 3 + 1] + n_tangent1[1];
        tgan[p1 * 3 + 2] = tgan[p1 * 3 + 2] + n_tangent1[2];
        tgan[p2 * 3] = tgan[p2 * 3] + n_tangent1[0];
        tgan[p2 * 3 + 1] = tgan[p2 * 3 + 1] + n_tangent1[1];
        tgan[p2 * 3 + 2] = tgan[p2 * 3 + 2] + n_tangent1[2];
        tgan[p3 * 3] = tgan[p3 * 3] + n_tangent1[0];
        tgan[p3 * 3 + 1] = tgan[p3 * 3 + 1] + n_tangent1[1];
        tgan[p3 * 3 + 2] = tgan[p3 * 3 + 2] + n_tangent1[2];
        bitangent1[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
        bitangent1[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
        bitangent1[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
        //printf("t: %f %f %f \n", bitangent1[0], bitangent1[1], bitangent1[2]);

        //normalize(bitangent1, n_bitangent1);
        n_bitangent1[0] = bitangent1[0];
        n_bitangent1[1] = bitangent1[1];
        n_bitangent1[2] = bitangent1[2];
        bitgan[p1 * 3] = bitgan[p1 * 3] + n_bitangent1[0];
        bitgan[p1 * 3 + 1] = bitgan[p1 * 3 + 1] + n_bitangent1[1];
        bitgan[p1 * 3 + 2] = bitgan[p1 * 3 + 2] + n_bitangent1[2];
        bitgan[p2 * 3] = bitgan[p2 * 3] + n_bitangent1[0];
        bitgan[p2 * 3 + 1] = bitgan[p2 * 3 + 1] + n_bitangent1[1];
        bitgan[p2 * 3 + 2] = bitgan[p2 * 3 + 2] + n_bitangent1[2];
        bitgan[p3 * 3] = bitgan[p3 * 3] + n_bitangent1[0];
        bitgan[p3 * 3 + 1] = bitgan[p3 * 3 + 1] + n_bitangent1[1];
        bitgan[p3 * 3 + 2] = bitgan[p3 * 3 + 2] + n_bitangent1[2];

        //printf("%d %d %d \n%f %f %f\n%f %f %f\n", shapes[0].mesh.indices[i], shapes[0].mesh.indices[i+1], shapes[0].mesh.indices[i+2], n_tangent1[0], n_tangent1[1], n_tangent1[2], n_bitangent1[0], n_bitangent1[1], n_bitangent1[2]);
        /*
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;
		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);
		*/
    }
    printf(" %d \n", v_n);

    for (int i = 0; i < v_n; i = i + 1)
    {
        float vex_C[3], vex_D[3];
        vex_C[0] = tgan[i * 3];
        vex_C[1] = tgan[i * 3 + 1];
        vex_C[2] = tgan[i * 3 + 2];
        normalize(vex_C, vex_D);

        tgan[i * 3] = vex_D[0];
        tgan[i * 3 + 1] = vex_D[1];
        tgan[i * 3 + 2] = vex_D[2];

        vex_C[0] = bitgan[i * 3];
        vex_C[1] = bitgan[i * 3 + 1];
        vex_C[2] = bitgan[i * 3 + 2];
        normalize(vex_C, vex_D);

        bitgan[i * 3] = vex_D[0];
        bitgan[i * 3 + 1] = vex_D[1];
        bitgan[i * 3 + 2] = vex_D[2];

        //vex_count[i] = 0;
        //printf("%d \n", vex_count[i]);
        //tgan[i * 3] = tgan[i * 3]/ vex_count[i];
        //tgan[i * 3 + 1] = tgan[i * 3 + 1] / vex_count[i];
        //tgan[i * 3 + 2] = tgan[i * 3 + 2] / vex_count[i];
        //bitgan[i * 3] = bitgan[i * 3] / vex_count[i];
        //bitgan[i * 3 + 1] = bitgan[i * 3 + 1] / vex_count[i];
        //bitgan[i * 3 + 2] = bitgan[i * 3 + 2] / vex_count[i];
        //if(bitgan[i * 3] >= 1 || bitgan[i * 3] <= -1)
        //printf("%d\n n: %f %f %f t: %f %f %f b: %f %f %f\n", tgan[i * 3], tgan[i * 3 + 1], tgan[i * 3 + 2]);
        printf("t: %f %f %f \n", tgan[i * 3], tgan[i * 3 + 1], tgan[i * 3 + 2]);
        printf("bt: %f %f %f \n", bitgan[i * 3], bitgan[i * 3 + 1], bitgan[i * 3 + 2]);
        printf("n: %f %f %f \n", shapes[0].mesh.normals[i * 3], shapes[0].mesh.normals[i * 3 + 1], shapes[0].mesh.normals[i * 3 + 2]);
    }

    if (flat_shading)
    {
        ret.computeFlatNormal();

        // Upload position array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ret.positions_flat.size(),
                     ret.positions_flat.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        if (ret.texcoords_flat.size() > 0)
        {

            // Upload texCoord array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ret.texcoords_flat.size(),
                         ret.texcoords_flat.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_uv = true;
        }

        if (ret.normals_flat.size() > 0)
        {
            // Upload normal array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * ret.normals_flat.size(),
                         ret.normals_flat.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_normal = true;
        }

        glBindVertexArray(0);
    }
    else
    {
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

        if (shapes[0].mesh.texcoords.size() > 0)
        {

            // Upload texCoord array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
                         shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_uv = true;
        }

        if (shapes[0].mesh.normals.size() > 0)
        {

            // Upload normal array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
                         shapes[0].mesh.normals.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[3]);

            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
                         tgan, GL_STATIC_DRAW);
            //printf("%d",shapes[0].mesh.normals.size());
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[4]);

            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
                         bitgan, GL_STATIC_DRAW);
            //printf("%d",shapes[0].mesh.normals.size());
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
    if (!flat_shading)
    {
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, numIndices);
    }
}

void StaticMesh::computeFlatNormal()
{
    glm::vec3 v1;
    glm::vec3 v2;

    /* 
        compute face normals by cross-product 
        (also fill in new postions (positions_flat), 
        and new normals (normals_flat),
        and new texture coordinates (texcoords_flat))
    */
    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3)
    {
        v1.x = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i + 1] * 3] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3];
        v1.y = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i + 1] * 3 + 1] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 1];
        v1.z = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i + 1] * 3 + 2] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 2];

        v2.x = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i + 2] * 3] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3];
        v2.y = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i + 2] * 3 + 1] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 1];
        v2.z = shapes[0].mesh.positions.data()[shapes[0].mesh.indices.data()[i + 2] * 3 + 2] - shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 2];

        for (int i = 0; i < 3; i++)
        {
            normals_flat.push_back(glm::normalize(glm::cross(v1, v2)).x);
            normals_flat.push_back(glm::normalize(glm::cross(v1, v2)).y);
            normals_flat.push_back(glm::normalize(glm::cross(v1, v2)).z);
        }

        // push new vertex positions
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 1]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i] * 3 + 2]);

        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i + 1] * 3]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i + 1] * 3 + 1]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i + 1] * 3 + 2]);

        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i + 2] * 3]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i + 2] * 3 + 1]);
        positions_flat.push_back(shapes[0].mesh.positions[shapes[0].mesh.indices.data()[i + 2] * 3 + 2]);

        // push new texture coordinates
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i] * 2]);
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i] * 2 + 1]);

        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i + 1] * 2]);
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i + 1] * 2 + 1]);

        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i + 2] * 2]);
        texcoords_flat.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices.data()[i + 2] * 2 + 1]);
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
