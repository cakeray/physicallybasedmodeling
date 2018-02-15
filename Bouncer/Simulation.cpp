#include "Simulation.h"

/*
    Euler approach to simulation
    v(n+1) = v(n) + a(n)h
    x(n+1) = x(n) + (v(n) + v(n+1))h/2
    where 
    v -> velocity as vec3
    x -> position of the ball as vec3
    h -> timestep
*/

void StartSimulation(glm::vec3 ballPosition) {
    //  initial conditions
    std::cout << "simulation started" << std::endl;
    glm::vec3 s_ballPosition = ballPosition;
    const glm::vec3 s_gravity = glm::vec3(0.0, -9.8, 0.0);
    glm::vec3 s_velocity = glm::vec3(10.0, 0.0, 0.0);
}

/*
    Updates the position at each frame taking into account different factors
*/
glm::vec3 UpdatePosition(glm::vec3 ballPosition) {
    static glm::vec3 velocity = glm::vec3(10, 15.8, 0);     //  starting velocity
    const glm::vec3 gravity = glm::vec3(0.0, -9.8, 0.0);    //  constant gravity
    static glm::vec3 position = ballPosition;               //  starting position
    static float h = 0.01;                                  //  timestep
    const float mass = 1.0;                                 //  mass of ball
    float airResistanceConstant = 0.5;                      //  constant for air resistance
    glm::vec3 windVelocity = glm::vec3(0, 0, 0);  //  wind velocity

    //  Calculating acceleration taking into account gravity and air resistance
    glm::vec3 acceleration = gravity + (airResistanceConstant / mass)*(windVelocity - velocity);

    //  Euler simulation
    glm::vec3 newVelocity = velocity + acceleration*h;
    glm::vec3 newPosition = position + h*((newVelocity + velocity)/2.0f);

    //if(newPosition.y>0)
      //  std::cout << newVelocity.x <<" "<< newVelocity.y <<" " << newVelocity.z <<"\t" << newPosition.x << " " << newPosition.y << " " <<newPosition.z << std::endl;

    //  Updating velocity and position for next frame
    velocity = newVelocity;
    position = newPosition;

    return newPosition * 0.001f;
}