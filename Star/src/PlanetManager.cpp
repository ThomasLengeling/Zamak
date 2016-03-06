//
//  PlanetManager.cpp
//  PlanetManager
//
//  Created by tom on 11/28/15.
//
//


#include "PlanetManager.h"

using namespace ci;

PlanetManager::PlanetManager()
{
    mNumPlanets		= 0;
    mCurrentPlanets = 0;
}

void PlanetManager::update( float dt )
{
    if( mCurrentPlanets < mNumPlanets ){
        addPlanet();
    } else if( mCurrentPlanets > mNumPlanets ){
        removePlanet();
    }
    
    if( mCurrentPlanets > 0 )
        updatePlanets( dt );
}

void PlanetManager::updatePlanets( float dt )
{
    for( auto & planet : mPlanets ){
        planet->update(  );
    }
}
void PlanetManager::addPlanet()
{
    /*
    float orbitRadius = mCurrentPlanets * mCurrentPlanets * Rand::randFloat( 100.0f, 150.0f ) + 100.0f + mRadius;
    float orbitSpeed  = 1.0f/orbitRadius * 4.0f;//Rand::randFloat( 0.001f, 0.01f );
    float radius	  = 3 + Rand::randFloat( mCurrentPlanets * 2, mCurrentPlanets * 4 );
    mPlanets.push_back( Planet( orbitRadius, orbitSpeed, radius ) );
    mCurrentPlanets ++;
    */
}

void PlanetManager::removePlanet()
{
    mPlanets.pop_back();
    mCurrentPlanets --;
}

void PlanetManager::draw()
{
    
}

void PlanetManager::drawOrbitRings()
{
    if( mCurrentPlanets > 0 ){
        gl::pushModelView();
        gl::rotate( 90, vec3( 1.0f, 0.0f, 0.0f ) );
        for( const auto & planets : mPlanets){
            gl::drawStrokedCircle( vec2(0), planets->getOrbitRadius()  );
        }
        gl::popModelView();
    }
}

void PlanetManager::drawPlanets( gl::GlslProg *shader )
{
    if( mCurrentPlanets > 0 ){
        //		gl::pushModelView();
        //		gl::rotate( vec3( 87.0f, 8.0f, 0.0f ) );
        for( const auto & planets : mPlanets){
            shader->uniform( "planetColor", planets->getColor() );
            planets->draw();
        }
        //		gl::popModelView();
    }
}

void PlanetManager::resetRadius()
{
   // mRadius = mRadiusDest * 0.5f;
}

void PlanetManager::randRadius()
{
    /*
    mRadiusDest = Rand::randFloat( 50.0f, 100.0f );
    mMass		= ( (mRadiusDest * mRadiusDest * mRadiusDest ) * (float)M_4_PI ) * 0.33333f;
     */
}

void PlanetManager::adjustRadius( float amt )
{
    //mRadiusDest = constrain( mRadiusDest + amt, 1.0f, mMaxRadius );
}

void PlanetManager::setRadius( float amt )
{
    //mRadiusDest = constrain( amt, 1.0f, mMaxRadius );
}

void PlanetManager::setColor( float amt )
{
    //mColorDest = amt;
}

void PlanetManager::setNumPlanets( int amt )
{
    mPlanets.clear();
    mCurrentPlanets = 0;
    mNumPlanets = amt;
}