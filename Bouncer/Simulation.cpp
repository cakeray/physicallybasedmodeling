//  Contains all the simulation related functions

#include "Functions.h"
#include "Globals.h"

//
//  Add gravity - default is (0.0,-9.8,0.0)
//
glm::vec3 add_gravity() {
    return glm::vec3(0.0, -9.8, 0.0);
}