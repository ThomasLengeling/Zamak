//
//  Controller.hpp
//  Star
//
//  Created by tom on 11/28/15.
//
//
#pragma once

#include "cinder/gl/Gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Batch.h"
#include "cinder/Rand.h"

#include "Glow.h"
#include "Nebula.h"

class Controller {
public:
    struct DustVertex {
    public:
        ci::vec3 vertex;
        ci::vec4 color;
    };
    
    struct Dust {
        Dust( const ci::vec3 &pos, const ci::vec3 &vel )
        : mPos( pos ), mVel( vel )
        {
            mPosInit		= mPos;
            mLifespanInit	= ci::Rand::randFloat( 5.0f, 12.0f );
            mLifespan		= mLifespanInit;
            mInvLifespan	= 0.1f;
            mAge			= 0.0f;
            mCol			= ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
        }
        
        void update( float dt ){
            mPos	+= mVel * dt;
            mAge	+= dt;
            
            if( mAge > mLifespan ){
                mLifespan += 10.0f;
                mPos = mPosInit;
            }
            mAgePer = 1.0 - ( mAge - mLifespan ) * mInvLifespan;
            
            mCol	= ci::ColorA( 1.0f, 1.0f, 1.0f, mAgePer * 0.15f );
        }
        
        ci::vec3 mPosInit, mPos, mVel;
        ci::ColorA mCol;
        float mAge, mLifespanInit, mLifespan, mInvLifespan, mAgePer;
        bool mIsDead;
    };
    
    Controller();
    
    void update( float dt );
    void updateDusts( float dt );
    void drawGlows( ci::gl::GlslProgRef shader, const ci::vec3 &right, const ci::vec3 &up );
    void drawNebulas( ci::gl::GlslProgRef shader, const ci::vec3 &right, const ci::vec3 &up );
    void drawDusts();
    
    void addGlows(  ci::vec3 mPos, float mRad, float mRadMulti, float power, int amt );
    void addNebulas(  ci::vec3 mPos, float mRad, float mRadMulti, int amt );
    void addDusts( ci::vec3 mPos, ci::vec3 mVel, float mRadius, int amt );
    
    // CANIS MAJORIS
    void addCMNebulas( const ci::vec3 &starPos, float starRadius, float radiusMulti, int amt );
    void addCMGlows( const ci::vec3 &starPos, float starRadius, int amt );
    
    std::vector<Glow>   getGlows(){return mGlows;}
    std::vector<Nebula> getNebulas(){return mNebulas;}
    
    std::vector<Glow>	mGlows;
    std::vector<Nebula>	mNebulas;
    std::vector<Dust>	mDusts;
    int					mTotalDustVerts;
    int					mPrevTotalDustVerts;
    DustVertex			*mDustVerts;
};