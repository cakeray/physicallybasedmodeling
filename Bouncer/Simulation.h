#pragma once

#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
//  GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//  Function prototypes
void StartSimulation(glm::vec3 ballPosition);
glm::vec3 UpdatePosition(glm::vec3 ballPosition);

#endif // !SIMULATION_H
