#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

//  Contains all the global variables required by the program

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
Camera camera(glm::vec3(0.0, 2.0, 10.0));
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


#endif // !GLOBALS_H

