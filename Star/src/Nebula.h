//
//  Nebula.hpp
//  Star
//
//  Created by tom on 11/28/15.
//
//

#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

class Nebula {
public:
    Nebula();
    Nebula( const ci::vec3 & pos, const ci::vec3 & vel, float radius, float lifespan );
    
    void update( float timeDelta );
    void draw( const ci::vec3 & right, const ci::vec3 & up );
    
    ci::vec3	mPos, mVel;
    float		mRadius, mRadiusDest, mRadiusMulti;
    float		mRot;
    float		mAge, mAgePer;
    float		mLifespan;
    bool		mIsDead;
    
};
