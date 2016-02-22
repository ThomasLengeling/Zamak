//
//  Glow.cpp
//  Star
//
//  Created by tom on 11/28/15.
//
//

#include "Glow.h"

using namespace ci;

Glow::Glow()
{
    
}

Glow::Glow( const vec3 & pos, const vec3 & vel, float radius, float lifespan )
{
    mPos		= pos;
    mVel		= vel;
    mRadiusDest	= radius;
    mRadius		= radius * 1.25;
    mAge		= 0.0f;
    mLifespan	= lifespan;
    mIsDead		= false;
    mRot		= Rand::randFloat( 360.0f );
    mRotVel		= Rand::randFloat( -10.0f, 10.0f );
    
    if( Rand::randFloat() < 0.01f ){
        mRadius *= 2.0f;
        mLifespan *= 1.3f;
        mRotVel	= Rand::randPosNegFloat( 1.0f, 3.0f );
    }
}

void Glow::update( float dt )
{
    mRadius -= ( mRadius - mRadiusDest ) * 0.1f * dt;
    mPos	+= mVel * dt;
    mVel	-= mVel * vec3(0.02) * dt;
    mAge	+= dt;
    mAgePer  = 1.0f - mAge/mLifespan;
    mRot	+= mRotVel * dt;
    //			mRadius *= 0.99f;
    
    if( mAge > mLifespan ){// || mRadius < 1.0f ){
        mIsDead = true;
        //				mRadius = 1.0f;
    }
}

void Glow::draw( const vec3 & right, const vec3 & up )
{
    gl::drawBillboard( mPos, ci::vec2( mRadius, mRadius ) * mAgePer, mRot, right, up );
}

