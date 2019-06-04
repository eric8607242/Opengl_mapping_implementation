#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
class ShadowMap {
    public:
        ShadowMap(const ShadowMap &sm)=default;

        static ShadowMap LoadDepth(const int shadow_width, const int shadow_height);
        void attach();
        void generate();
        void bindToChannel(GLuint channel);
        unsigned int getdepthMap();
    private:
        ShadowMap();
        unsigned int depthMapFBO;
        unsigned int depthMap;

        int shadow_width;
        int shadow_height;

};

#endif
