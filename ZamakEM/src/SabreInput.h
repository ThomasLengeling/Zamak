//
//  SabreStream.hpp
//  ZamakEM
//
//  Created by tom on 1/17/16.
//
//

#pragma once

#include <tuple>

namespace sabre {

    static int MAX_KEYS = 25;
    
    struct SabreInput{
        std::vector< std::tuple<std::string, bool, float> >    mKeys;
        std::tuple< std::string, bool, ci::vec3 >              mAccelero;
        std::tuple< std::string, bool, ci::vec3 >              mGyro;
        std::tuple< std::string, bool, ci::vec3 >              mMagneto;
        std::tuple< std::string, bool, float >                 mHeading;
        std::tuple< std::string, bool, float >                 mTilt;
        std::tuple< std::string, bool, ci::ivec3 >             mBottons;
        std::tuple< std::string, bool, float >                 mPressure;
        std::tuple< std::string, bool, float >                 mKeyCode;
        std::tuple< std::string, bool, float >                 mNode;
    };
}