//
//  ZamakEMApp.h
//  ZamakEM
//
//  Created by tom on 1/17/16.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CinderImGui.h"

#include "Event.h"



class ZamakEMApp : public ci::app::App {
public:
    void setup() override;
    void mouseDown( ci::app::MouseEvent event ) override;
    void update() override;
    void draw() override;
    
    //Events
    void updateEvents();
    
     zamak::Event makeSabreEvent(std::string name);
    
private:
    
    
    std::vector<zamak::Event> mEvents;
};

