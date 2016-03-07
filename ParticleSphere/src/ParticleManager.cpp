//
//  ParticleManager.cpp
//  ParticleSphere
//
//  Created by tom on 3/6/16.
//
//

#include "ParticleManager.h"

using namespace ci;

//---------  Particle
Particle::Particle()
{
    mTime = 0.0;
}

void Particle::update()
{
    
}

void Particle::draw()
{
    gl::color(1, 1, 1);
    gl::drawSphere(mPos, 0.1, 12);
}


//---------  ParticleManager


ParticleManager::ParticleManager()
{
    
}

void ParticleManager::update()
{
    for(auto & particle : mParticles){
        particle->update();
    }
}

void ParticleManager::draw()
{
    for(auto & particle : mParticles){
        particle->draw();
    }
}


void ParticleManager::drawPathBetweenVectors()
{
    for(const auto & partFrom : mParticles){
        for(const auto & partTo : mParticles){
            //if( distance(partFrom->getPos(), partTo->getPos()) < 50 ){
                drawPathSphere(partFrom, partTo);
            //}
        }
    }
}

void ParticleManager::drawTranstionPath(Particle * p0, Particle * p1)
{
    gl::drawSphere(slerp(p0->getPos(), p1->getPos(), t), );
}

void ParticleManager::drawPathSphere(Particle * p0, Particle * p1)
{
 
    
    gl::VertBatch vert(GL_LINE_STRIP);
    gl::color(1, 0, 1);
    for(float t = 0.0f; t < 1.0f; t += .05f){
        vert.vertex(slerp(p0->getPos(), p1->getPos(), t));
    }
    vert.draw();
}

void ParticleManager::addParticle(Particle * p)
{
    mParticles.push_back(p);
}