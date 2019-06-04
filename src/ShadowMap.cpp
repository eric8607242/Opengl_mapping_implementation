#include "ShadowMap.h"

ShadowMap::ShadowMap()
{

}

ShadowMap ShadowMap::LoadDepth(const int shadow_width, const int shadow_height)
{
    ShadowMap sm;
    sm.shadow_width = shadow_width;
    sm.shadow_height = shadow_height;

    glGenFramebuffers(1, &sm.depthMapFBO);

    glGenTextures(1, &sm.depthMap);
    glBindTexture(GL_TEXTURE_2D, sm.depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 sm.shadow_width, sm.shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return sm;
}

void ShadowMap::attach()
{
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::generate()
{
    glViewport(0, 0, shadow_width, shadow_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

unsigned int ShadowMap::getdepthMap()
{
    return depthMap;
}

void ShadowMap::bindToChannel(GLuint channel)
{
    glActiveTexture(GL_TEXTURE0+channel);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}
