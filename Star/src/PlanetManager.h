//
//  Star.hpp
//  Star
//
//  Created by tom on 11/28/15.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/gl/GlslProg.h"

#include "Planet.h"

class PlanetManager{
    
public:
    
    PlanetManager();
    
    void update( float dt );
    void updatePlanets( float dt );
    void draw();
    void drawOrbitRings();
    void drawPlanets( ci::gl::GlslProg *shader );
    void resetRadius();
    void randRadius();
    void adjustRadius( float amt );
    void setRadius( float amt );
    void setColor( float amt );
    void setNumPlanets( int amt );
    void addPlanet();
    void removePlanet();
    
private:
    
    int			mCurrentPlanets;
    int			mNumPlanets;
    std::vector<PlanetRef>	mPlanets;
};