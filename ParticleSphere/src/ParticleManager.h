//
//  ParticleManager.hpp
//  ParticleSphere
//
//  Created by tom on 3/6/16.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

class Particle{
public:
    Particle();
    
    void update();
    void draw();
    
    void setPosition(ci::vec3 pos){mPos = pos;}
    const ci::vec3 & getPos(){return mPos;}
    
    float getTime(){return mTime;}
    
private:
    
    ci::vec3 mPos;
    ci::vec3 mVel;
    
    float    mTime;
    float    mTimeInc;
    float    mDir;
    
};


//-----------------------
class ParticleManager;
typedef std::shared_ptr<ParticleManager>  ParticleManagerRef;

class ParticleManager{
public:
    
    ParticleManager();
    
    static ParticleManagerRef  create(){
        return std::make_shared<ParticleManager>();
    }
    
    void update();
    void draw();
    
    void addParticle(Particle * p);
    
    void drawPathBetweenVectors();

    void drawPathSphere(Particle * p0, Particle * p1);
    void drawTranstionPath(Particle * p0, Particle * p1);
    
private:
    std::vector<Particle *> mParticles;
    
    ci::gl::BatchRef        mCircleBatch;
};