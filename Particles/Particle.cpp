/*
    Implementation of PARTICLE.H
*/

#include "Particle.h"

Particle::Particle()
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