//
//  ParticlesStream.hpp
//  ZamakEM
//
//  Created by tom on 1/17/16.
//
//
#pragma once

#include "cinder/Color.h"

namespace  zamak {
    
    struct ParticleStream{
        std::tuple< std::string, bool, float > mSize;
        std::tuple< std::string, bool, ci::Color > mColor;
    };
    
    struct VideoStream{
        
    };
}