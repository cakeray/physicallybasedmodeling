/*
    Implementation of PARTICLE_EMITTER_H
*/

#include "ParticleEmitter.h"
#include <iostream>

ParticleEmitter::ParticleEmitter(int maxCount, glm::vec3 pos, glm::vec3 vel, float velVariance, float life, float lifeVariance) :
    m_maxCount(maxCount), m_position(pos), m_velocity(vel), m_velocityVariance(velVariance), m_life(life), m_lifeVariance(lifeVariance)
{
    m_particles = new Particle[m_maxCount];
    Initialize();
}

ParticleEmitter::~ParticleEmitter()
{
    delete[] m_particles;
}

void ParticleEmitter::Initialize() 
{
    for (int i = 0; i < m_maxCount; i++)
    {
        m_particles[i].m_position = m_position;
        m_particles[i].m_velocity = m_velocity;
        m_particles[i].m_life = m_life;
        m_particles[i].m_alive = true;
        m_particles[i].m_pid = i;
    }
}

void ParticleEmitter::AddForce(const glm::vec3& gravity)
{
    //  timestep, h=0.01;
    float h = 0.01f;

    //  Update position and velocity of each particle in the m_particles array
    //  Decrease life by 0.01;
    //  if life <= 0.0, set m_alive to false
    //  push index onto stack
    for (int i = 0; i < m_maxCount; i++)
    {
        glm::vec3 pos = m_particles[i].m_position;
        glm::vec3 vel = m_particles[i].m_velocity;

        glm::vec3 acceleration = gravity;

        glm::vec3 newVel = vel + acceleration*h;
        glm::vec3 newPos = pos + h*((newVel - vel) / 2.0f);

        m_particles[i].m_position = newPos;
        m_particles[i].m_velocity = newVel;

        //  Reduce life
        m_particles[i].m_life -= 1.0f;

        if (m_particles[i].m_life <= 0.0f) 
        {
            m_particles[i].m_alive = false;
            RestartDead(i);
        }
    }
}

void ParticleEmitter::RestartDead(int i)
{
    m_particles[i].m_position = m_position;
    m_particles[i].m_velocity = m_velocity;
    m_particles[i].m_life = m_life;
    m_particles[i].m_alive = true;
    m_particles[i].m_pid = i;
}

void ParticleEmitter::PrintDetails()
{
    for (int i = 0; i < m_maxCount; i++)
    {
        std::cout << "Particle ID: " << m_particles[i].m_pid<<std::endl;
        std::cout << "Position: " << m_particles[i].m_position.x <<" "<< m_particles[i].m_position.y<<" "<< m_particles[i].m_position.z << std::endl;
        std::cout << "Velocity: " << m_particles[i].m_velocity.x<<" "<< m_particles[i].m_velocity.y << " " << m_particles[i].m_velocity.z << std::endl;
        std::cout << "Life: " << m_particles[i].m_life << std::endl;
        std::cout << "Alive? " << m_particles[i].m_alive << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }
}