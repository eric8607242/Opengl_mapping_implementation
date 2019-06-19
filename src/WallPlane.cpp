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

    // wall plane definition: every 6 vertices defines a wallPlane

    // iterate through 6 faces
    for (int i = 0; i < cubeMesh.normals_flat.size(); i+=18) {

        if (cubeMesh.normals_flat[i] == 1) { // normal x vector > 0
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (!maze -> BLOCKED[m_i][m_j].posX) {
                        for (int k = 0; k < 18; k+=3) {
                            translatedPos.push_back(cubeMesh.positions_flat[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.normals_flat[i+k]);
                            normals.push_back(cubeMesh.normals_flat[i+k+1]);
                            normals.push_back(cubeMesh.normals_flat[i+k+2]);
                        }

                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        } else if (cubeMesh.normals_flat[i] == -1) { // normal x vector < 0
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (!maze -> BLOCKED[m_i][m_j].negX) {
                        for (int k = 0; k < 18; k+=3) {
                            translatedPos.push_back(cubeMesh.positions_flat[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.normals_flat[i+k]);
                            normals.push_back(cubeMesh.normals_flat[i+k+1]);
                            normals.push_back(cubeMesh.normals_flat[i+k+2]);
                        }
                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        } else if (cubeMesh.normals_flat[i+2] == 1) { // normal z vector > 0
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (!maze -> BLOCKED[m_i][m_j].posZ) {
                        for (int k = 0; k < 18; k+=3) {
                            translatedPos.push_back(cubeMesh.positions_flat[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.normals_flat[i+k]);
                            normals.push_back(cubeMesh.normals_flat[i+k+1]);
                            normals.push_back(cubeMesh.normals_flat[i+k+2]);
                        }

                        // store to wallPlanes
                        wallPlanes.push_back(WallPlane(translatedPos, normals));
                        translatedPos.clear();
                        normals.clear();
                    }
                }
            }
        } else if (cubeMesh.normals_flat[i+2] == -1) { // normal z vector < 0
            // check blockage
            for (int m_i = 0; m_i < MAZE_ROW; m_i++) {
                for (int m_j = 0; m_j < MAZE_COL; m_j++) {
                    if (!maze -> BLOCKED[m_i][m_j].negZ) {
                        for (int k = 0; k < 18; k+=3) {
                            translatedPos.push_back(cubeMesh.positions_flat[i+k] + m_i);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+1] + 0.5f);
                            translatedPos.push_back(cubeMesh.positions_flat[i+k+2] + m_j);
                            
                            normals.push_back(cubeMesh.normals_flat[i+k]);
                            normals.push_back(cubeMesh.normals_flat[i+k+1]);
                            normals.push_back(cubeMesh.normals_flat[i+k+2]);
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
    

    return glm::abs(glm::dot(vecPlaneToPt, N));
}