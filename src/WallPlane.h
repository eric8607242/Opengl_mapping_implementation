#ifndef _WALLPLANE_H_
#define _WALLPLANE_H_

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include "StaticMesh.h"
#include "Maze.h"

#define MAZE_ROW 17
#define MAZE_COL 14

class WallPlane {
    public:
        WallPlane(std::vector<GLfloat> vertices_pos, std::vector<GLfloat> normals);
        std::vector<GLfloat> wall_vertices;
        std::vector<GLfloat> wall_normals;

        void printWallVertices();
        void printWallNormals();

        static std::vector<WallPlane> storeWallPlanes(Maze *maze, StaticMesh &cubeMesh);
        static GLfloat distToWallPlane(glm::vec3 const &point, WallPlane const &wallplane);
};



#endif
