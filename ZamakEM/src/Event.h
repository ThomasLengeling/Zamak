//
//  Event.hpp
//  ZamakEM
//
//  Created by tom on 1/17/16.
//
//


#pragma once

#include "SabreInput.h"
#include "Toggles.h"


namespace zamak{
    
    class Event{
    public:
        
        std::string                 mEventName;
        
        sabre::SabreInput           mSabreStream;
        zamak::ParticleStream       mParticleStream;
        
        
    private:

        
    };
}
