#ifndef _WALLPLANE_H_
#define _WALLPLANE_H_

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

class WallPlane {
    public:
        WallPlane(std::vector<GLfloat> vertices_pos);
        std::vector<GLfloat> vertices;

        void showPlanes();
};



#endif
