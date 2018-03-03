#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

//  GLM
#include <glm/glm.hpp>

class Particle
{
public:
    Particle();
    ~Particle();

    bool IsAlive();

private:
    glm::vec3 m_position;       //  position
    glm::vec3 m_velocity;       //  velocity
    int m_pid;                  //  particle id
    float m_mass;               //  mass
    float m_life;               //  life span
    bool m_alive;               //  dead or alive

    friend class ParticleEmitter;
};


#endif // !PARTICLE_H
