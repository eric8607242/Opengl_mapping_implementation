#ifndef _MAZE_H_
#define _MAZE_H_

#define MAZE_ROW 17
#define MAZE_COL 14

#include "ShaderProgram.h"
#include "StaticMesh.h"
#include "ScopeResource.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct blockage {
    bool posX;
    bool negX;
    bool posZ;
    bool negZ;
};

class Maze {

    public:
        Maze();
        void init(int maze_map[MAZE_ROW][MAZE_COL]);
        void drawMaze(Program &shader, StaticMesh &cubeMesh);


        int map[MAZE_ROW][MAZE_COL];
        blockage BLOCKED[MAZE_ROW][MAZE_COL];
        
};

#endif