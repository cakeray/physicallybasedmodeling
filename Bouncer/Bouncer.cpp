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
#include "Simulation.h"


//  Callback function definitions
void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double x_offSet, double y_offSet);

//  Shape functions
void RenderSphere();
void RenderBox();


//  Screen
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

//  Shape variables
GLuint sphereVAO = 0;
GLuint indexCount;
unsigned int cubeVBO, cubeVAO;

//  Ball variables
glm::vec3 ballPosition(0.0, 0.0, 0.0);      //  Specifies the initial position

//  Time
float deltaTime = 0.0;
float lastFrame = 0.0;

//  Camera
Camera camera(glm::vec3(0.0, 0.0, 50.0));   //  Specifies the initial position
float lastX = SCREEN_WIDTH / 2.0;
float lastY = SCREEN_HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
bool mouseClickActive = false;

//  Shaders
Shader ball;
Shader box;

//  Textures
Texture boxTex;

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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    //  Load Shader
    ball.LoadShader("ball.vert", "ball.frag");
    box.LoadShader("box.vert", "box.frag");
    
    boxTex.LoadTexture("images/tiles.jpg", "boxTex");

    StartSimulation(ballPosition);

    float h = 0.01f;                                            //  timestep
    float fraction = 1.0f;                                      //  fraction
    glm::vec3 velocity = glm::vec3(30.0f, 10.8f, 80.0f);        //  starting velocity
    const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);     //  constant gravity
    const float mass = 1.0f;                                    //  mass of ball
    float airResistanceConstant = 0.5f;                         //  constant for air resistance
    glm::vec3 windVelocity = glm::vec3(0.0f, 0.0f, 0.0f);       //  wind velocity

    //  RENDER LOOP
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

        //  Set ball shader
        ball.Use();
        ball.SetMat4("projection", projection);
        ball.SetMat4("view", view);

        glm::mat4 ballModel;
        ballModel = glm::translate(ballModel, ballPosition);
        ballModel = glm::scale(ballModel, glm::vec3(1.0f));
        ball.SetMat4("model", ballModel);
        //  render sphere
        RenderSphere();
        //ballPosition = UpdatePosition(ballPosition);
        
        //  Calculating acceleration taking into account gravity and air resistance
        //glm::vec3 acceleration = gravity + (airResistanceConstant / mass) * (windVelocity - velocity);
        glm::vec3 acceleration = gravity;
        //std::cout << "Acceleration: "<<acceleration.x << " " << acceleration.y << " " << acceleration.z << std::endl;
        //  Euler simulation
        glm::vec3 newVelocity = velocity + acceleration*h;
        glm::vec3 newPosition = ballPosition + h*((newVelocity + velocity) / 2.0f);

        //std::cout << "Incoming velocity: " << newVelocity.x << " " << newVelocity.y << " " << newVelocity.z << std::endl;
        if (CollisionCheck(newPosition)) {
            //std::cout << "------------" << std::endl;
            //std::cout << "Collision occured" << std::endl;
            float dCurrent = FindDistance(ballPosition);
            float dNext = FindDistance(newPosition);

            fraction = dCurrent / (dCurrent - dNext);
            //h = fraction * h;
            //std::cout << h << std::endl;

            velocity = CollisionResponse(newVelocity);
            ballPosition = ballPosition;

            //std::cout << "------------" << std::endl;
        }
        else {
            //  Updating velocity and position for next frame
            velocity = newVelocity;
            ballPosition = newPosition;

            //  Resetting timestep
            //h = h / fraction;
        }


        //  Set box shader
        box.Use();
        glm::mat4 boxModel;
        box.SetMat4("projection", projection);
        box.SetMat4("view", view);
        //  model matrix Set
        boxModel = glm::scale(boxModel, glm::vec3(14.5f));
        box.SetMat4("model", boxModel);
        //  bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxTex.GetTextureID());
        // render the cube
        RenderBox();

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


/*
    Renders a sphere
*/
void RenderSphere()
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

/*
    Renders the box
*/
void RenderBox() {
    float box_vertices[] = {
        // Back face
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
        1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f,  1.0f, 0.0f, // bottom-right         
        1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
                                          // Front face
                                          -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left
                                          1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
                                          1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
                                          1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
                                          -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, // top-left
                                          -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left                                 
                                                                            // Left face                                                                    
                                                                            -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-right                                                                    
                                                                            -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-left
                                                                            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-left
                                                                            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-left
                                                                            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-right
                                                                            -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-right
                                                                                                              // Right face
                                                                                                              1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
                                                                                                              1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-right
                                                                                                              1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right         
                                                                                                              1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-right
                                                                                                              1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
                                                                                                              1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left     
                                                                                                                                               // Bottom face
                                                                                                                                               -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // top-right
                                                                                                                                               1.0f, -1.0f, -1.0f,  1.0f, 1.0f, // top-left
                                                                                                                                               1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-left
                                                                                                                                               1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-left
                                                                                                                                               -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-right
                                                                                                                                               -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // top-right
                                                                                                                                                                                 // Top face
                                                                                                                                                                                 -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
                                                                                                                                                                                 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
                                                                                                                                                                                 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right     
                                                                                                                                                                                 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
                                                                                                                                                                                 -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
                                                                                                                                                                                 -1.0f,  1.0f,  1.0f,  0.0f, 0.0f  // bottom-left                                                                   
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
