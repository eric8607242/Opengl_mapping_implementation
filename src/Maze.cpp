#include "Maze.h"

Maze::Maze() {

}

void Maze::init(int maze_map[MAZE_ROW][MAZE_COL]) {

    // init map and blockage
    for (int i = 0; i < MAZE_ROW; i++) {
        for(int j = 0; j < MAZE_COL; j++) {
            map[i][j] = maze_map[i][j];

            if (i - 1 < 0 || map[i-1][j] == 1) {
                BLOCKED[i][j].negX = true;
            }

            if (i + 1 > MAZE_ROW - 1 || map[i+1][j] == 1) {
                BLOCKED[i][j].posX = true;
            }

            if (j - 1 < 0 || map[i][j-1] == 1) {
                BLOCKED[i][j].negZ = true;
            }

            if (j + 1 > MAZE_COL - 1 || map[i][j+1] == 1) {
                BLOCKED[i][j].posZ = true;
            }
        }
    }

}

void Maze::drawMaze(Program &shader, StaticMesh &cubeMesh)
{

    for (int i = 0; i < MAZE_ROW; i++)
    {
        for (int j = 0; j < MAZE_COL; j++)
        {
            if (map[i][j] == 1)
            {
                shader["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.5f, j)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f));
                cubeMesh.draw();               
            }
        }
    }
  
}
