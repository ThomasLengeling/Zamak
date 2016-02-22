//
//  Glow.hpp
//  Star
//
//  Created by tom on 11/28/15.
//
//

#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

class Glow {
public:
    
    Glow();
    Glow( const ci::vec3 &pos, const ci::vec3 & vel, float radius, float lifespan );
    void update( float timeDelta );
    void draw( const ci::vec3 &right, const ci::vec3 & up );
    
    ci::vec3	mPos;
    ci::vec3	mVel;
    float		mRot, mRotVel;
    float		mRadius, mRadiusDest;
    float		mAge, mAgePer, mLifespan;
    float		mColor;
    bool		mIsDead;
};