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


//  Callback function definitions
void process_input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

//  Shape functions
void render_sphere();
void render_box();

//  Simulation functions
glm::vec3 add_gravity();


//  Screen
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//  Shape variables
GLuint sphereVAO = 0;
GLuint indexCount;
unsigned int cubeVBO, cubeVAO;

//  Ball variables
glm::vec3 ball_position(0.0, 0.0, 0.0);

//  Time
float delta_time = 0.0;
float last_frame = 0.0;

//  Camera
Camera camera(glm::vec3(0.0, 1.0, 15.0));
float last_x = SCREEN_WIDTH / 2.0;
float last_y = SCREEN_HEIGHT / 2.0;
bool keys[1024];
bool first_mouse = true;
bool mouse_click_active = false;

//  Shaders
Shader ball;
Shader box;

//  Textures
Texture box_tex;

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
        ball_position += add_gravity();

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

//
//  Whenever the window size is changed (automatically by OS, or manually by user) this function is called
//
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//
//  Processes all input
//  When input keys are pressed or camera is moved, this function reacts acccordingly
//
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
}

//
//  Whenever the mouse moves, this function is called  
//
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    if (first_mouse) {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    if (mouse_click_active)
        camera.ProcessMouseMovement(x_offset, y_offset);
}

//
//  Activates mouse when left button is clicked
//
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouse_click_active = true;
    else
        mouse_click_active = false;
}

//
//  Whenever mouse scroll wheel is used, this function is called
//
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    camera.ProcessMouseScroll(y_offset);
}


//
//  Renders a sphere
//
void render_sphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        GLuint vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<GLuint> indices;

        const GLuint X_SEGMENTS = 64;
        const GLuint Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (GLuint y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (GLuint x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

//
//  renders the box
//
void render_box() {
    float box_vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                                          // Front face
                                          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
                                          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                                          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
                                          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
                                          -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
                                          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left                                 
                                                                            // Left face                                                                    
                                                                            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right                                                                    
                                                                            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
                                                                            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
                                                                            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
                                                                            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
                                                                            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
                                                                                                              // Right face
                                                                                                              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
                                                                                                              0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
                                                                                                              0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
                                                                                                              0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
                                                                                                              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
                                                                                                              0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
                                                                                                                                               // Bottom face
                                                                                                                                               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
                                                                                                                                               0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
                                                                                                                                               0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
                                                                                                                                               0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
                                                                                                                                               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
                                                                                                                                               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
                                                                                                                                                                                 // Top face
                                                                                                                                                                                 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                                                                                                                                                                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                                                                                                                                                                                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
                                                                                                                                                                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                                                                                                                                                                                 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                                                                                                                                                                                 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left                                                                   
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    float stride = (3 + 2) * sizeof(float);
    //  position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    //  texture coordinate attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //  render the box
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

//
//  Add gravity - default is (0.0,-9.8,0.0)
//
glm::vec3 add_gravity() {
    return glm::vec3(0.0, -9.8, 0.0) * 0.0009f;
}
