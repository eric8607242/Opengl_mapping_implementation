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
void processInput(GLFWwindow *window);
void renderScene(Program &shader, StaticMesh &mesh, StaticMesh &cubeMesh, float degree);
static void error_callback(int error, const char *description)
{
    std::cerr << fmt::format("Error: {0}\n", description);
}

Camera camera(glm::vec3(0.0f, 1.0f, 7.0f));

// define wall face objects
std::vector<WallPlane> wallPlanes;

// maze object
Maze *maze;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup style
    ImGui::StyleColorsDark();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    auto text = Texture2D::LoadFromFile("../resource/image.png");
    auto mesh = StaticMesh::LoadMesh("../resource/sphere.obj");
    auto cubeMesh = StaticMesh::LoadMesh("../resource/cube.obj");

    auto prog = Program::LoadFromFile(
        "../resource/vs.vert",
        "../resource/gs.geom",
        "../resource/fs.frag");
    auto prog2 = Program::LoadFromFile(
        "../resource/vs.vert",
        "../resource/gs.geom",
        "../resource/fs_light.frag");
    auto depthshader = Program::LoadFromFile(
        "../resource/depth.vert",
        "../resource/depth.frag");
    auto shadow = ShadowMap::LoadDepth(shadow_width, shadow_height);
    shadow.attach();

    float planeVertices[] = {
        25.0f, 25.0f, -1.5f, 25.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -25.0f, 25.0f, -1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -25.0f, -25.0f, -1.5f, 0.0f, 25.0f, 0.0f, 0.0f, 1.0f,

        25.0f, 25.0f, -1.5f, 25.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -25.0f, -25.0f, -1.5f, 0.0f, 25.0f, 0.0f, 0.0f, 1.0f,
        25.0f, -25.0f, -1.5f, 25.0f, 25.0f, 0.0f, 0.0f, 1.0f};

    

    // plane vertex buffer configuration
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
    glBindVertexArray(0);

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

        // maze initialize
        maze -> init(map);

        // store horizontal wall planes (not including blocked ones)
        wallPlanes = WallPlane::storeWallPlanes(maze, cubeMesh);


        float degree = 0.0f;
        glm::vec3 object_color{1.0f};

        float specular_s = 0.0f;
        float ambient_s = 0.0f;

        prog.use();
        prog["text"] = 0;
        prog["shadowMap"] = 1;

        glEnable(GL_DEPTH_TEST);

        while (!glfwWindowShouldClose(window))
        {
            glm::vec3 light_pos = camera.Position;
            glm::vec3 light_center = camera.Position + camera.Front;

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            glfwPollEvents();

            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 1. render depth of scene to texture
            float near_plane = 0.1f, far_plane = 10.0f;

            glm::mat4 lightprojection, lightview;
            glm::mat4 lightspacematrix;
            //lightprojection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, near_plane, far_plane);
            lightprojection = glm::perspective(90.0f / 180.0f * 3.1415926f, (GLfloat)1024 / (GLfloat)1024, near_plane, far_plane);
            //lightview = glm::lookAt(light_pos, camera.Front, glm::vec3(0.0, 1.0, 0.0));
            lightview = camera.GetViewMatrix();
            lightspacematrix = lightprojection * lightview;
            depthshader.use();
            depthshader["lightspacematrix"] = lightspacematrix;

            shadow.generate();
            renderScene(depthshader, mesh, cubeMesh, degree);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(0, 0, display_w, display_h);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            prog.use();
            prog["lightspacematrix"] = lightspacematrix;
            prog["vp"] = glm::perspective(45 / 180.0f * 3.1415926f, 1024.0f / 768.0f, 0.1f, 10000.0f) * camera.GetViewMatrix();
            prog["object_color"] = object_color;

            prog["light_pos"] = light_pos;
            prog["eye_pos"] = glm::vec3{0, 0, 10}; // set to the entrance of maze
            prog["specular_s"] = specular_s;
            prog["ambient_s"] = ambient_s;
            prog["cutoff"] = glm::cos(glm::radians(15.0f));
            prog["light_center"] = light_center;

            text.bindToChannel(0);
            shadow.bindToChannel(1);
            renderScene(prog, mesh, cubeMesh, degree);
            {
                prog2["vp"] = glm::perspective(45 / 180.0f * 3.1415926f, 1024.0f / 768.0f, 0.1f, 10000.0f) *
                              glm::lookAt(glm::vec3{0, 0, 10}, glm::vec3{0, 0, 10}, glm::vec3{0, 1, 0});
                prog2["model"] = glm::translate(glm::mat4(1.0f), light_pos) * glm::scale(glm::mat4(1.0f), glm::vec3{0.2f});

                prog2.use();
                //mesh.draw();
            }

            GLfloat xMin, xMax, zMin, zMax;
            // collision detection
            for (int i = 0; i < wallPlanes.size(); i++) {
                xMin = wallPlanes[i].wall_vertices[0];
                xMax = wallPlanes[i].wall_vertices[3];
                zMin = wallPlanes[i].wall_vertices[2];
                zMax = wallPlanes[i].wall_vertices[5];
                
                if (xMin > xMax) {
                    std::swap(xMin, xMax);
                }

                if (zMin > zMax) {
                    std::swap(zMin, zMax);
                }
                
                bool inXRange = (zMin == zMax) && (camera.Position.x >= xMin && camera.Position.x <= xMax);
                bool inZRange = (xMin == xMax) && (camera.Position.z >= zMin && camera.Position.z <= zMax);
                
                if (WallPlane::distToWallPlane(camera.Position, wallPlanes[i]) <= 0.2) {
                    // also inside wall plane X or Z min/max bound ??
                    if (inXRange || inZRange) {
                        std::cout << "Collided. " << i << std::endl;
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

                ImGui::SliderFloat("degree", &degree, 0.0f, 360.0f); // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("Specular", &specular_s, 0, 1);
                ImGui::SliderFloat("Ambient", &ambient_s, 0, 1);
                ImGui::ColorEdit3("clear color", (float *)&clear_color);         // Edit 3 floats representing a color
                ImGui::ColorEdit3("object color", glm::value_ptr(object_color)); // Edit 3 floats representing a color
                ImGui::SliderFloat2("Position", glm::value_ptr(light_center), -10, 10);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

void renderScene(Program &shader, StaticMesh &mesh, StaticMesh &cubeMesh, float degree)
{
    // draw plane
    shader["model"] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), 270.0f * 3.1415926f / 180.0f, glm::vec3(1, 0, 0));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);


    // draw maze
    maze -> drawMaze(shader, cubeMesh);
  
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

