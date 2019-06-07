#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#define GLM_FORCE_RADIAN
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <memory>
#include "utils/imgui_impl_opengl3.h"
#include "utils/imgui_impl_glfw.h"

class CameraFPS {
    public:
        UpdateView();
        GetViewMatrix();
        KeyPressed(const unsigned char key);
        MouseMove(int x, int y, int width, int height);
}