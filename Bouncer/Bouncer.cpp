/*
    Rushil Kekre
    Bouncer.cpp
    Bouncing ball simulation
    C++, OpenGL, GLSL
    11/05/2017
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
#include "Globals.h"
#include "Functions.h"

int main() {

    //  GLFW: Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
       
    //  GLFW: Window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"PBM",NULL,NULL);
    if (window==NULL) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //  Set callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //  GLAD: Initialization
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //  Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    //  Load Shader
    ball.loadShader("ball.vert", "ball.frag");
    box.loadShader("box.vert", "box.frag");
    
    box_tex.loadTexture("images/checkerboard.jpg", "box_tex");


    //  RENDER LOOP
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        //  Process input
        process_input(window);

        glClearColor(0.2, 0.2, 0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //  projection and view matrix set
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //  set ball shader
        ball.Use();
        ball.setMat4("projection", projection);
        ball.setMat4("view", view);
        glm::mat4 ball_model;
        ball_model = glm::translate(ball_model, ball_position);
        ball_model = glm::scale(ball_model, glm::vec3(0.25f));
        ball.setMat4("model", ball_model);
        //  render sphere
        render_sphere();
        ball_position += add_gravity() * glm::vec3(0.001);

        //  set box shader
        box.Use();
        glm::mat4 box_model;
        box.setMat4("projection", projection);
        box.setMat4("view", view);
        //  model matrix set
        box_model = glm::scale(box_model, glm::vec3(8.0f));
        box.setMat4("model", box_model);
        //  bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box_tex.getTextureID());
        // render the cube
        render_box();

        //  Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //  terminate
    glfwTerminate();
    return 0;
}


