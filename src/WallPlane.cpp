#include "WallPlane.h"

WallPlane::WallPlane(std::vector<GLfloat> vertices_pos, std::vector<GLfloat> normals) 
    : wall_vertices(vertices_pos), wall_normals(normals)
{

}

void WallPlane::printWallVertices() {
    for (int i = 0; i < wall_vertices.size(); i+=3) {
        std::cout << wall_vertices[i] << " " << wall_vertices[i+1] << " " << wall_vertices[i+2] << std::endl;
    }
}

void WallPlane::printWallNormals() {
    for (int i = 0; i < wall_normals.size(); i+=3) {
        std::cout << wall_normals[i] << " " << wall_normals[i+1] << " " << wall_normals[i+2] << std::endl;
    }
}

std::vector<WallPlane> WallPlane::storeWallPlanes(int maze[MAZE_ROW][MAZE_COL], StaticMesh &cubeMesh) {
    std::vector<GLfloat> translatedPos;
    std::vector<GLfloat> normals;
    std::vector<WallPlane> wallPlanes;
    for (int i = 0; i < cubeMesh.smooth_n.size() - 3; i+=3) {
        // wall plane definition:
        // x, z noraml vector != 0 and the that dircetion isn't blocked by other cubes

        if ((cubeMesh.smooth_n[i] != 0) || cubeMesh.smooth_n[i+2] != 0) {
            for (int maze_i = 0; maze_i < MAZE_ROW; maze_i++) {
                for (int maze_j = 0; maze_j < MAZE_COL; maze_j++) {

                    

                    // store wall vertex positions
                    translatedPos.push_back(cubeMesh.smooth_position[i] + maze_i);
                    translatedPos.push_back(cubeMesh.smooth_position[i+1] + 0.5f);
                    translatedPos.push_back(cubeMesh.smooth_position[i+2] + maze_j);

                    // store wall normals
                    normals.push_back(cubeMesh.smooth_n[i]);
                    normals.push_back(cubeMesh.smooth_n[i+1]);
                    normals.push_back(cubeMesh.smooth_n[i+2]);
                    
                    
                    wallPlanes.push_back(WallPlane(translatedPos, normals));
                    translatedPos.clear();
                    normals.clear();
                }
            }
        }
    }

    return wallPlanes;
}