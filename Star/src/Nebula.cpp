//
//  Nebula.cpp
//  Star
//
//  Created by tom on 11/28/15.
//
//

#include "Nebula.h"


using namespace ci;

Nebula::Nebula()
{
}

Nebula::Nebula( const vec3 & pos, const vec3 & vel, float radius, float lifespan )
{
    mPos			= pos;
    mVel			= vel;
    mRadiusDest		= radius;
    mRadius			= radius;
    mAge			= 0.0f;
    mLifespan		= Rand::randFloat( lifespan * 0.5f, lifespan );
    mRadiusMulti	= 3.0f;
    mRot			= Rand::randFloat( 360.0f );
    mAgePer			= 0.0f;
    mIsDead			= false;
}

void Nebula::update( float dt )
{
    mPos	+= mVel * dt;
    mVel	-= mVel * vec3(0.01) * dt;
    mRadiusDest += mRadiusMulti * dt;
    mRadiusMulti -= mRadiusMulti * 0.03f * dt;
    
    mRadius -= ( mRadius - mRadiusDest ) * 0.1f * dt;
    
    mAge += dt;
    mAgePer = 1.0f - mAge/mLifespan;//sin( ( mAge/mLifespan ) * M_PI );
    
    if( mAge > mLifespan ){
        mIsDead = true;
    }
}

void Nebula::draw( const vec3 &right, const vec3 &up )
{
    gl::drawBillboard( mPos, vec2( mRadius, mRadius ), mRot, right, up );
}
