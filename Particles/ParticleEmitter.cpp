/*
    Implementation of PARTICLE_EMITTER_H
*/

#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter(int maxCount, glm::vec3 pos, glm::vec3 vel, float velVariance, float life, float lifeVariance) :
    m_maxCount(maxCount), m_position(pos), m_velocity(vel), m_velocityVariance(velVariance), m_life(life), m_lifeVariance(lifeVariance)
{
    initialize();
}

ParticleEmitter::~ParticleEmitter()
{
    delete[] m_particles;
}

void ParticleEmitter::initialize() 
{
    for (int i = 0; i < m_maxCount; i++)
    {
        m_particles[i].m_position = m_position;
        m_particles[i].m_velocity = m_velocity;
        m_particles[i].m_life = m_life;
    }
}