#pragma once

#ifndef SIMULATION_H
#define SIMULATION_H

//  Headers
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//  Function prototypes
void StartSimulation(glm::vec3 ballPosition);
void ConfigurePlanes();
glm::vec3 UpdatePosition(glm::vec3 ballPosition);
bool CollisionCheck(glm::vec3 position);

#endif // !SIMULATION_H
