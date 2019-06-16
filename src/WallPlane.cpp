#include "WallPlane.h"

WallPlane::WallPlane(std::vector<GLfloat> vertices_pos, std::vector<GLfloat> normals)
    : wall_vertices(vertices_pos), wall_normals(normals)
{
}

void WallPlane::printWallVertices()
{
    for (int i = 0; i < wall_vertices.size(); i += 3)
    {
        std::cout << wall_vertices[i] << " " << wall_vertices[i + 1] << " " << wall_vertices[i + 2] << std::endl;
    }
}

void WallPlane::printWallNormals()
{
    for (int i = 0; i < wall_normals.size(); i += 3)
    {
        std::cout << wall_normals[i] << " " << wall_normals[i + 1] << " " << wall_normals[i + 2] << std::endl;
    }
}

std::vector<WallPlane> WallPlane::storeWallPlanes(Maze *maze, StaticMesh &cubeMesh)
{
    std::vector<GLfloat> translatedPos;
    std::vector<GLfloat> normals;
    std::vector<WallPlane> wallPlanes;

    // wall plane definition: every 4 vertices defines a wallPlane

    for (int i = 0; i < cubeMesh.smooth_n.size(); i += 12) {

        
        if (cubeMesh.smooth_n[i] > 0) { // normal x vector > 0 (horizontal +X)
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (maze -> BLOCKED[m_i][m_j].posX == false) {
                        for (int k = 0; k < 12; k+=3) {
                            translatedPos.push_back(cubeMesh.smooth_position[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.smooth_n[i+k]);
                            normals.push_back(cubeMesh.smooth_n[i+k+1]);
                            normals.push_back(cubeMesh.smooth_n[i+k+2]);
                        }
                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        } else if (cubeMesh.smooth_n[i] < 0) { // normal x vector < 0 (horizontal -X)
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (maze -> BLOCKED[m_i][m_j].negX == false) {
                        for (int k = 0; k < 12; k+=3) {
                            translatedPos.push_back(cubeMesh.smooth_position[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.smooth_n[i+k]);
                            normals.push_back(cubeMesh.smooth_n[i+k+1]);
                            normals.push_back(cubeMesh.smooth_n[i+k+2]);
                        }
                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        } else if (cubeMesh.smooth_n[i+2] > 0) { // normal z vector > 0 (horizontal +Z)
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (maze -> BLOCKED[m_i][m_j].posZ == false) {
                        for (int k = 0; k < 12; k+=3) {
                            translatedPos.push_back(cubeMesh.smooth_position[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.smooth_n[i+k]);
                            normals.push_back(cubeMesh.smooth_n[i+k+1]);
                            normals.push_back(cubeMesh.smooth_n[i+k+2]);
                        }
                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        } else if (cubeMesh.smooth_n[i+2] < 0) { // normal z vector < 0 (horizontal -Z)
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (maze -> BLOCKED[m_i][m_j].negZ == false) {
                        for (int k = 0; k < 12; k+=3) {
                            translatedPos.push_back(cubeMesh.smooth_position[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.smooth_position[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.smooth_n[i+k]);
                            normals.push_back(cubeMesh.smooth_n[i+k+1]);
                            normals.push_back(cubeMesh.smooth_n[i+k+2]);
                        }
                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        }
    }

    return wallPlanes;
}

GLfloat WallPlane::distToWallPlane(glm::vec3 const &point, WallPlane const &wallplane) 
{
    glm::vec3 vecPlaneToPt = point - glm::vec3(wallplane.wall_vertices[0], wallplane.wall_vertices[1], wallplane.wall_vertices[2]);
    glm::vec3 N = glm::vec3(wallplane.wall_normals[0], wallplane.wall_normals[0], wallplane.wall_normals[2]);
    

    return glm::abs((GLfloat)glm::dot(vecPlaneToPt, N));
}