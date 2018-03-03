/*
Rushil Kekre
ParticleSim.cpp
Particle simulation
C++, OpenGL, GLSL
03/01/2018
*/

//  OpenGL headers
#include <glad/glad.h>
#include <glfw/glfw3.h>

//  GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//  C++ headers
#include <iostream>

//  Custon headers
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "ParticleEmitter.h"

//  Callback function definitions
void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double x_offSet, double y_offSet);

//  Screen
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//  Time
float deltaTime = 0.0;
float lastFrame = 0.0;

//  Camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));   //  Specifies the initial position
float lastX = SCREEN_WIDTH / 2.0;
float lastY = SCREEN_HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
bool mouseClickActive = false;

int main() {

    //  GLFW: Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //  GLFW: Window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particles", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //  Set callback functions
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    //  GLAD: Initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //  Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //  Environment properties
    glm::vec3 gravity(0.0f, -9.8f, 0.0f);

    //  Particle properties
    int pCount = 50000;
    glm::vec3 position(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity(5.0f, 0.0f, 0.0f);
    float velocityVariance = 0.0f;
    float life = 10.0f;
    float lifeVariance = 0.0f;

    //  Creating particle sim object
    ParticleEmitter pSim(pCount,position,velocity,velocityVariance,life,lifeVariance);

    while (!glfwWindowShouldClose(window)) {

        //  per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //  Process input
        ProcessInput(window);

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //  projection and view matrix Set
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //  Simulation takes place here
        pSim.AddForce(gravity);
        pSim.PrintDetails();

        //  Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //  terminate
    glfwTerminate();
    return 0;
}

/*
    Whenever the window size is changed (automatically by OS, or manually by user) this function is called
*/
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/*
    Processes all input
    When input keys are pressed or camera is moved, this function reacts acccordingly
*/
void ProcessInput(GLFWwindow* window) {
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

/*
    Whenever the mouse moves, this function is called
*/
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
    if (firstMouse) {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }

    float x_offSet = x_pos - lastX;
    float y_offSet = lastY - y_pos;

    lastX = x_pos;
    lastY = y_pos;

    if (mouseClickActive)
        camera.ProcessMouseMovement(x_offSet, y_offSet);
}

/*
    Activates mouse when left button is clicked
*/
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouseClickActive = true;
    else
        mouseClickActive = false;
}

/*
    Whenever mouse scroll wheel is used, this function is called
*/
void ScrollCallback(GLFWwindow* window, double x_offSet, double y_offSet) {
    camera.ProcessMouseScroll(y_offSet);
}

