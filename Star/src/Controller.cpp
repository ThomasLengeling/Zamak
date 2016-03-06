//
//  Controller.cpp
//  Star
//
//  Created by tom on 11/28/15.
//
//


#include "Controller.h"

const int MAX_DUSTS = 20000;

using namespace ci;
using std::vector;

Controller::Controller()
{
    mPrevTotalDustVerts	= -1;
    mTotalDustVerts		= 0;
    mDustVerts			= NULL;
}

void Controller::update( float dt )
{
    // GLOWS
    for( vector<Glow>::iterator it = mGlows.begin(); it != mGlows.end(); ){
        if( it->mIsDead ){
            it = mGlows.erase( it );
        } else {
            it->update( dt );
            ++ it;
        }
    }
    
    // NEBULAS
    for( vector<Nebula>::iterator it = mNebulas.begin(); it != mNebulas.end(); ){
        if( it->mIsDead ){
            it = mNebulas.erase( it );
        } else {
            it->update( dt );
            ++ it;
        }
    }
    
    updateDusts( dt );
}

void Controller::updateDusts( float dt )
{
    for( vector<Dust>::iterator it = mDusts.begin(); it != mDusts.end(); ){
        if( it->mIsDead ){
            it = mDusts.erase( it );
        } else {
            it->update( dt );
            it++;
        }
    }
    
    mTotalDustVerts = mDusts.size();
    
    if (mTotalDustVerts != mPrevTotalDustVerts) {
        if (mDustVerts != NULL) {
            delete[] mDustVerts;
        }
        mDustVerts = new DustVertex[mTotalDustVerts];
        mPrevTotalDustVerts = mTotalDustVerts;
    }
    
    int vIndex = 0;
    for( vector<Dust>::iterator it = mDusts.begin(); it != mDusts.end(); ++it ){
        mDustVerts[vIndex].vertex = it->mPos;
        mDustVerts[vIndex].color  = vec4( it->mCol.r, it->mCol.g, it->mCol.b, it->mCol.a );
        vIndex++;
    }
}

void Controller::drawGlows( gl::GlslProgRef shader, const vec3 &right, const vec3 &up )
{
    for( vector<Glow>::iterator it = mGlows.begin(); it != mGlows.end(); ++it ){
        shader->uniform( "alpha", it->mAgePer );
        it->draw( right, up );
    }
}

void Controller::drawNebulas( gl::GlslProgRef shader, const vec3 &right, const vec3 &up )
{
    for( vector<Nebula>::iterator it = mNebulas.begin(); it != mNebulas.end(); ++it ){
        shader->uniform( "alpha", it->mAgePer );
        it->draw( right, up );
    }
}

void Controller::drawDusts()
{
    if( mTotalDustVerts > 0 ){
        gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
        gl::VertBatch vert(GL_POINTS);
        for(int i = 0; i < mTotalDustVerts; i++){
            vert.vertex(mDustVerts[i].vertex);
            vert.color(mDustVerts[i].color);
        }
        vert.draw();
    }
}

void Controller::addGlows( ci::vec3 mPos, float mRad, float mRadMulti, float power, int amt )
{
    for( int i = 0; i < amt; i++ ){
        float radius	= Rand::randFloat( 15.0f, 20.0f ) * mRadMulti;
        vec3 dir		= Rand::randVec3();
        vec3 pos		= mPos + dir * vec3(( mRad - radius * 0.25 ));
        vec3 vel		= dir * Rand::randFloat( 1.3f, 2.0f );
        
        float lifespan	= Rand::randFloat( 15.0f, 30.0f ) * mRadMulti;
        
        mGlows.push_back( Glow( pos, vel, radius + ( 1.0f - power ) * 35.0f, lifespan ) );
    }
}

void Controller::addNebulas( ci::vec3 mPos, float mRad, float mRadMulti, int amt )
{
    for( int i=0; i<amt; i++ ){
        float radius		= Rand::randFloat( 15.0f, 25.0f ) * mRadMulti;
        vec3 dir			= Rand::randVec3();
        vec3 pos			= mPos + dir * vec3(( mRad - radius * 0.25f ));
        vec3 vel			= dir * Rand::randFloat( 0.2f, 1.0f ) * mRadMulti;
        float lifespan		= Rand::randFloat( 35.0f, 55.0f ) * mRadMulti;
        mNebulas.push_back( Nebula( pos, vel, radius, lifespan ) );
    }
}

void Controller::addDusts( ci::vec3 mPos, ci::vec3 mVel, float mRadius, int amt )
{
    int dustsToMake = amt;
    if( mDusts.size() + amt > MAX_DUSTS ){
        dustsToMake = MAX_DUSTS - ( mDusts.size() + amt );
    }
    
    if( dustsToMake > 0 ){
        for( int i=0; i < dustsToMake; i++ ){
            vec3 dir   = Rand::randVec3();
            vec3 pos	= mPos + dir * mRadius;
            vec3 vel	= mVel + Rand::randVec3() + dir * Rand::randFloat( 0.85f, 1.5f );
            mDusts.push_back( Dust( pos, vel ) );
        }
    }
}

void Controller::addCMNebulas( const vec3 &cmPos, float cmRadius, float radiusMulti, int amt )
{
    for( int i=0; i<amt; i++ ){
        float angle		= Rand::randFloat( -0.25f, 0.25f );
        float cosA		= cos( angle );
        float sinA		= sin( angle );
        vec3 dir		= vec3( cosA, sinA, 0.0f );
        vec3 pos		= -cmPos + dir * cmRadius;
        
        vec3 vel		= dir * Rand::randFloat( 1.2f, 3.0f );
        float radius	= Rand::randFloat( 35.0f, 50.0f ) * radiusMulti;
        float lifespan	= Rand::randFloat( 35.0f, 55.0f );
        mNebulas.push_back( Nebula( pos, vel, radius, lifespan ) );
    }
}

void Controller::addCMGlows( const vec3 &cmPos, float cmRadius, int amt )
{
    for( int i=0; i<amt; i++ ){
        float angle		= Rand::randFloat( -0.25f, 0.25f );
        float cosA		= cos( angle );
        float sinA		= sin( angle );
        vec3 dir		= vec3( cosA, sinA, 0.0f );
        vec3 pos		= -cmPos + dir * cmRadius;
        
        vec3 vel		= dir * Rand::randFloat( 4.5f, 8.0f );
        float radius	= Rand::randFloat( 75.0f, 150.0f ) * 2.0f;
        float lifespan	= Rand::randFloat( 35.0f, 55.0f );
        mGlows.push_back( Glow( pos, vel, radius, lifespan ) );
    }
}
