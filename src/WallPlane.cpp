#include "WallPlane.h"

WallPlane::WallPlane(std::vector<GLfloat> vertices_pos) 
    : vertices(vertices_pos)
{

}

void WallPlane::showPlanes() {
    for (int i = 0; i < vertices.size(); i+=3) {
        std::cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;
    }
}