#include "Simulation.h"


//  Global Variables
std::vector<glm::vec3> planePositions(6);   //  DS to store plane positions and normals used for collision detection
std::vector<glm::vec3> planeNormals(6);
std::vector<bool> collisionPlane(6, false);


void StartSimulation(glm::vec3 ballPosition) {
    //  initial conditions
    std::cout << "simulation started" << std::endl;
    glm::vec3 s_ballPosition = ballPosition;

    ConfigurePlanes();
    std::cout << "plane information created" << std::endl;

    std::cout << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

}

/*
Assigns the plane positions and the plane normals to the inputs.
Information stored in the following manner
[0] ->  +X  ->  RIGHT
[1] ->  -X  ->  LEFT
[2] ->  +Y  ->  TOP
[3] ->  -Y  ->  BOTTOM
[4] ->  +Z  ->  FRONT
[5] ->  -Z  ->  BACK
*/
void ConfigurePlanes() {
    planePositions[0] = glm::vec3(15.0f, 0.0, 0.0);
    planePositions[1] = glm::vec3(-15.0f, 0.0, 0.0);
    planePositions[2] = glm::vec3(0.0, 15.0f, 0.0);
    planePositions[3] = glm::vec3(0.0, -15.0f, 0.0);
    planePositions[4] = glm::vec3(0.0, 0.0, 15.0f);
    planePositions[5] = glm::vec3(0.0, 0.0, -15.0f);

    glm::vec3 negative(-1.0);
    planeNormals[0] = glm::normalize(planePositions[0] * negative) ;
    planeNormals[1] = glm::normalize(planePositions[1] * negative) ;
    planeNormals[2] = glm::normalize(planePositions[2] * negative) ;
    planeNormals[3] = glm::normalize(planePositions[3] * negative) ;
    planeNormals[4] = glm::normalize(planePositions[4] * negative) ;
    planeNormals[5] = glm::normalize(planePositions[5] * negative) ;
}

/*
    Updates the position at each frame taking into account different factors

    Euler approach to simulation
    v(n+1) = v(n) + a(n)h
    x(n+1) = x(n) + (v(n) + v(n+1))h/2
    where
    v -> velocity as vec3
    x -> position of the ball as vec3
    h -> timestep
*/
glm::vec3 UpdatePosition(glm::vec3 ballPosition) {
    float h = 0.01f;                                            //  timestep
    float f = 1.0f;                                             //  fraction
    glm::vec3 velocity = glm::vec3(5.0f, 10.8f, 0.0f);          //  starting velocity
    const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);     //  constant gravity
    const float mass = 1.0f;                                    //  mass of ball
    float airResistanceConstant = 0.5f;                         //  constant for air resistance
    glm::vec3 windVelocity = glm::vec3(-4.0f, 0.0f, 0.0f);      //  wind velocity

    //  Calculating acceleration taking into account gravity and air resistance
    glm::vec3 acceleration = gravity + (airResistanceConstant / mass)*(windVelocity - velocity);
    //  Euler simulation
    glm::vec3 newVelocity = velocity + acceleration*h;
    glm::vec3 newPosition = ballPosition + h*((newVelocity + velocity) / 2.0f);
    
    //  Check for collisions
    if (CollisionCheck(ballPosition)) {
        //std::cout << "Collision occured" << std::endl;
        ballPosition = ballPosition;
    }
    else {
        //std::cout << "Finding new positions" << std::endl;
        //  Updating velocity and position for next frame
        velocity = newVelocity;
        ballPosition = newPosition;
    }

    return ballPosition;
    
    //if(newPosition.y>0)
    //  std::cout << newVelocity.x <<" "<< newVelocity.y <<" " << newVelocity.z <<"\t" << newPosition.x << " " << newPosition.y << " " <<newPosition.z << std::endl;

}

/*
    Checks for collisions with the walls of the cube and returns true if collision occurs
*/
bool CollisionCheck(glm::vec3 position) {
    //std::cout << position.x << " " << position.y << " " << position.z << std::endl;
    float radius = 1.25f;
    
    for (int i = 0; i < planePositions.size(); i++) {
        glm::vec3 difference = position - planePositions[i];
        //std::cout << "Diff: " << difference.x <<" "<<difference.y<<" "<<difference.z << std::endl;
        float dotProd = glm::dot(difference, planeNormals[i]);
        //std::cout << dotProd << std::endl;
        float distance = dotProd - radius;
        //std::cout << distance << std::endl;

        if (distance < 0) {
            collisionPlane[i] = true;
            return true;
        }
    }
    
    //  if not returned from for loop, return false since no collision detected
    return false;
}

float FindDistance(glm::vec3 position) {
    float distance = FLT_MAX;
    float radius = 1.25f;

    for (int i = 0; i < planePositions.size(); i++) {
        glm::vec3 difference = position - planePositions[i];
        //std::cout << "Diff: " << difference.x <<" "<<difference.y<<" "<<difference.z << std::endl;
        float dotProd = glm::dot(difference, planeNormals[i]);
        //std::cout << dotProd << std::endl;
        float newDistance = dotProd - radius;
        //std::cout << distance << std::endl;

        distance = std::min(distance, newDistance);
    }

    return distance;
}

glm::vec3 CollisionResponse(glm::vec3 velocity) {
    int planeIndex = 0;
    float coe = 1.0f;       //  Coefficient of Elasticity
    float cof = 0.1f;       //  Coefficient of Friction
    
    //  Find the right plane where collision takes place
    for (int i = 0; i < collisionPlane.size(); i++) {
        if (collisionPlane[i])
            planeIndex = i;
    }
    glm::vec3 normal = planeNormals[planeIndex];

    //  Calculate normal and tangential velocity
    glm::vec3 normalVelocity = dot(velocity, normal) * normal;
    glm::vec3 tangentVelocity = velocity - normalVelocity;

    //  Calculate elastic and frictional veclocities
    glm::vec3 elasticVelocity = -1.0f * coe * normalVelocity;
    glm::vec3 frictionVelocity = (1.0f - cof) * tangentVelocity;

    //  Calculate new velocity
    glm::vec3 newVelocity = elasticVelocity + frictionVelocity;

    return newVelocity;
}