//render dock
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
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
#include "main.h"



unsigned int planeVAO, cubeVAO;
int shadow_width = 1024;
int shadow_height = 1024;
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void cursor_callback(GLFWwindow *window);
void processInput(GLFWwindow *window);
void renderScene(Program &shader, StaticMesh &floor, StaticMesh &mesh, StaticMesh &cubeMesh);
bool inRange(GLfloat low, GLfloat high, GLfloat x);
static void error_callback(int error, const char *description)
{
    std::cerr << fmt::format("Error: {0}\n", description);
}

Camera camera(glm::vec3(0.0f, 0.5f, 7.0f));

Maze *maze;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lamp_pos = glm::vec3(0.0f, 5.0f, 0.0f);
glm::vec3 lamp_center = glm::vec3(7.0f, 0.0f, 8.5f);

float near_plane = 0.1f, far_plane = 100.0f;
int alt_flag = 0;

bool normal_use = false;
bool parallax_use = false;

int main(void)
{
    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL())
    {
        exit(EXIT_FAILURE);
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup style
    ImGui::StyleColorsDark();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    auto text = Texture2D::LoadFromFile("../resource/wall.png");
    auto textNormal = Texture2D::LoadFromFile("../resource/wall_NRM.png");
    auto textDisp = Texture2D::LoadFromFile("../resource/wall_DISP.png");
    auto mesh = StaticMesh::LoadMesh("../resource/sphere.obj", false);
    auto cubeMesh = StaticMesh::LoadMesh("../resource/cube.obj", false);
    auto FloorMesh = StaticMesh::LoadMesh("../resource/floor.obj", false);

    auto prog = Program::LoadFromFile(
        "../resource/vs.vert",
        "../resource/gs.geom",
        "../resource/fs.frag");

    auto depthshader = Program::LoadFromFile(
        "../resource/depth.vert",
        "../resource/depth.frag");
    auto shadow = ShadowMap::LoadDepth(shadow_width, shadow_height);
    auto lamp_shadow = ShadowMap::LoadDepth(shadow_width, shadow_height);
    shadow.attach();
    lamp_shadow.attach();


    maze = new Maze();
    int map[MAZE_ROW][MAZE_COL] = {
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1}
    };

    {
        // text and mesh, shader => garbage collector
        auto g1 = Protect(text);
        auto g2 = Protect(mesh);
        auto g3 = Protect(prog);
        auto g4 = Protect(cubeMesh);
        auto g5 = Protect(FloorMesh);

        if (!mesh.hasNormal() || !mesh.hasUV())
        {
            std::cerr << "sphereMesh does not have normal or uv\n";
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (!cubeMesh.hasNormal() || !cubeMesh.hasUV())
        {
            std::cerr << "cubeMesh does not have normal or uv\n";
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (!FloorMesh.hasNormal() || !FloorMesh.hasUV())
        {
            std::cerr << "FloorMesh does not have normal or uv\n";
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        GLfloat playerRadius = 0.1f;

        maze -> init(map);

        glm::vec3 object_color{1.0f};

        prog.use();
        prog["text"] = 0;
        prog["flashshadowMap"] = 1;
        prog["lampshadowMap"] = 2;
        prog["text_normal"] = 3;
        prog["text_disp"] = 4;

        glEnable(GL_DEPTH_TEST);

        glm::mat4 lightprojection = glm::perspective(90.0f / 180.0f * 3.1415926f, (GLfloat)1024 / (GLfloat)1024, near_plane, far_plane);
        glm::mat4 lamplightview = glm::lookAt(lamp_pos, lamp_center, glm::vec3(1.0, 0.0, 0.0));

        while (!glfwWindowShouldClose(window))
        {
            cursor_callback(window);

            glm::vec3 prevCamPos = camera.Position;

            glm::vec3 light_pos = camera.Position + camera.light_offset;
            glm::vec3 light_center = camera.Position + camera.Front;

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glfwPollEvents();

            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 lightview;
            glm::mat4 lightspacematrix;

            lightview = camera.GetLightMatrix();
            lightspacematrix = lightprojection * lightview;
            depthshader.use();
            depthshader["lightspacematrix"] = lightspacematrix;

            shadow.generate();
            renderScene(depthshader, FloorMesh, mesh, cubeMesh);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            glm::mat4 lamplightspacematrix;

            lamplightspacematrix = lightprojection * lamplightview;
            depthshader.use();
            depthshader["lightspacematrix"] = lamplightspacematrix;

            lamp_shadow.generate();
            renderScene(depthshader, FloorMesh, mesh, cubeMesh);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(0, 0, display_w, display_h);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            prog.use();
            prog["lightspacematrix"] = lightspacematrix;
            prog["lamplightspacematrix"] = lamplightspacematrix;
            prog["vp"] = glm::perspective(45 / 180.0f * 3.1415926f, 1024.0f / 768.0f, 0.1f, 10000.0f) * camera.GetViewMatrix();
            prog["object_color"] = object_color;

            prog["light_pos"] = light_pos;
            prog["lamp_pos"] = lamp_pos;
            prog["lamp_center"] = lamp_center;
            prog["eye_pos"] = camera.Position; // set to the entrance of maze
            prog["cutoff"] = glm::cos(glm::radians(15.0f));
            prog["light_center"] = light_center;


            prog["normal_use"] = static_cast<int>(normal_use);
		    prog["parallax_use"] = static_cast<int>(parallax_use);


            text.bindToChannel(0); 
            shadow.bindToChannel(1);
            lamp_shadow.bindToChannel(2);
            textNormal.bindToChannel(3);
            textDisp.bindToChannel(4);

            renderScene(prog, FloorMesh, mesh, cubeMesh);

            // collision detection w/ bbox checking 
            GLfloat xLow, xHigh, zLow, zHigh, floorHeight;
            floorHeight = 0.3f;
            for (int i = 0; i < MAZE_ROW; i++) {
                for (int j = 0; j < MAZE_COL; j++) {
                    xLow = (-0.5 + i) - playerRadius;
                    xHigh = (0.5 + i) + playerRadius;
                    zLow = (-0.5 + j) - playerRadius;
                    zHigh = (0.5 + j) + playerRadius;
                    
                    if ((inRange(xLow, xHigh, camera.Position.x) && 
                        inRange(zLow, zHigh, camera.Position.z) &&
                        maze -> map[i][j] == 1) ||
                        camera.Position.y <= floorHeight) 
                    {
                        std::cout << "wall." << i << " " << j << std::endl;
                        camera.Position = prevCamPos;
                    }
                }
            }


            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                
                
                ImGui::Checkbox("normal mapping", &normal_use);
                ImGui::Checkbox("parallax mapping", &parallax_use);
                ImGui::End();
            }
 

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            glfwSwapBuffers(window);
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void renderScene(Program &shader, StaticMesh &floor, StaticMesh &mesh, StaticMesh &cubeMesh)
{
    // draw maze
    maze -> drawMaze(shader, cubeMesh); 

    // draw plane
    shader["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3{10.0f}) * glm::rotate(glm::mat4(1.0f), 270.0f * 3.1415926f / 180.0f, glm::vec3(1, 0, 0));
    shader["normal_use"] = 0;
    shader["parallax_use"] = 0;
    floor.draw(false);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void cursor_callback(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS){
        if(alt_flag == 0){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            alt_flag = 1;
        }
        else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, mouse_callback);
            alt_flag = 0;
        }
    }
}

// Returns true if x is in range [low..high], else false 
bool inRange(GLfloat low, GLfloat high, GLfloat x) { 
    return ((x-high) * (x-low) <= 0);
} 

