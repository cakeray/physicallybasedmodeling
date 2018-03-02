#pragma once
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "Particle.h"

class ParticleEmitter
{
public:
    ParticleEmitter(int maxCount, glm::vec3 pos, glm::vec3 vel, float velVariance, float life, float lifeVariance);
    ~ParticleEmitter();

private:
    //  member variables
    int m_maxCount;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float m_velocityVariance;
    float m_life;
    float m_lifeVariance;

    //  Particle array
    Particle* m_particles;

    //  member functions
    void initialize();
};

#endif // !PARTICLE_EMITTER_H

