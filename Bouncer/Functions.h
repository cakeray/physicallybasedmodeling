#pragma once

//  Contains all the function prototypes used by the program

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//  OpenGL headers
#include <glad/glad.h>
#include <glfw/glfw3.h>

//  GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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


#endif // !FUNCTIONS_H

