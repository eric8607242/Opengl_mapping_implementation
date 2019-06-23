#include "Maze.h"

Maze::Maze() {

}

void Maze::init(int maze_map[MAZE_ROW][MAZE_COL]) {

    // init map and blockage
    for (int i = 0; i < MAZE_ROW; i++) {
        for(int j = 0; j < MAZE_COL; j++) {
            map[i][j] = maze_map[i][j];

            if (i - 1 < 0) {
                BLOCKED[i][j].negX = true;
            } else {
                if (maze_map[i-1][j] == 1) {
                    BLOCKED[i][j].negX = true;
                }
            }

            if (i + 1 > MAZE_ROW - 1) {
                BLOCKED[i][j].posX = true;
            } else {
                if (maze_map[i+1][j] == 1) {
                    BLOCKED[i][j].posX = true;
                }
            }

            if (j - 1 < 0) {
                BLOCKED[i][j].negZ = true;
            } else {
                if (maze_map[i][j-1] == 1) {
                    BLOCKED[i][j].negZ = true;
                }
            }

            if (j + 1 > MAZE_COL - 1) {
                BLOCKED[i][j].posZ = true;
            } else {
                if (maze_map[i][j+1] == 1) {
                    BLOCKED[i][j].posZ = true;
                } 
            }
        }
    }

}

void Maze::drawMaze(Program &shader, StaticMesh &cubeMesh) {
    for (int i = 0; i < MAZE_ROW; i++) {
        for (int j = 0; j < MAZE_COL; j++) {
            if (map[i][j] == 1) {
                shader["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0f, j));
                cubeMesh.draw(true);              
            }
        }
    }
}

void Maze::printBlockage(int i, int j) {
    std::cout << std::boolalpha;
    std::cout << "-X: " << BLOCKED[i][j].negX << std::endl <<
    "+X: " << BLOCKED[i][j].posX << std::endl <<
    "-Z: " << BLOCKED[i][j].negZ << std::endl <<
    "+Z: " << BLOCKED[i][j].posZ << std::endl;
}
