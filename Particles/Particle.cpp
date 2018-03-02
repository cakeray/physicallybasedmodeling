/*
    Implementation of PARTICLE.H
*/

#include "Particle.h"

Particle::Particle() : m_position(glm::vec3(0.0, 0.0, 0.0)), m_velocity(glm::vec3(0.0,0.0,0.0)), m_mass(1.0), m_life(100.0), m_alive(true)
{
}

Particle::~Particle()
{
}

bool Particle::IsAlive() 
{
    if (m_life <= 0.0f)
        return false;
    else
        return true;
}