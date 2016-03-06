//
//  Planet.hpp
//  Star
//
//  Created by tom on 2/22/16.
//
//

#pragma once


#include "Cinder/Rand.h"
#include "Cinder/gl/gl.h"
#include "Cinder/gl/Batch.h"
#include "Cinder/gl/GlslProg.h"
#include "Cinder/app/App.h"

#include "Controller.h"

class Planet;
typedef std::shared_ptr<Planet> PlanetRef;

class Planet{
public:
    
    Planet();
    
    static PlanetRef create(){
        return std::make_shared<Planet>();
    }
    
    void setOrbits(float orbitRadius, float orbitSpeed, float radius);
    
    void setRadius(float radius){mRadius = radius;}
    void setPos(ci::vec3 pos){mPos = pos;}
    
    void setMass(float mass);
    
    void enableLights(){mLights = 1.0;}
    void disableLights(){mLights = 0.0;}
    
    //UPDATE
    void update();
    void updateRotation( float dt );
    void updateParticles();
    void updateTime();
    
    void draw();
    
    //Render passes
    void drawRenderPass();
    void drawCorona();
    void drawSphere();
    void drawGlows();
    void drawNebulas();
    void drawDust();
    void drawCircle();
    
    
    // SHADERS  and   TEXTURES
    void loadTextures();
    void loadShaders();
    
    void createSphere();

    void setBillboardVectors(const ci::vec3 & right, const ci::vec3  & up){mRight = right; mUp =up;}
    
    float getOrbitRadius(){return mOrbitRadius; }
    
    float getColor(){return mColor;}
    
    //Random
    void  randSeed();
    
private:
    ci::vec3	mPos;
    
    float		mOrbitSpeed;
    ci::vec3    mOrbitPos;
    
    float		mAngle;
    ci::vec3	mVel;
    
    float		mRadius;
    float		mRadiusDest;
    float       mRadiusDrawn;
    float		mRadiusMulti;
    float		mMaxRadius;
    float		mOrbitRadius;
    
    float		mMass;
    
    float		mColor;
    float		mColorDest;
    
    float       mStartColor;
    float       mCircleBrightness;
    float       mCoronaColor;
    float       mDustColor;
    float       mGlowColor;
    float       mCircleColor;
    float       mNebulaColor;
    
    float       mLights;
    
    ci::vec3    mRight;
    ci::vec3    mUp;
    
    // CONTROLLER
    Controller			mController;
    
    
    ci::gl::BatchRef	mSphereBatch;
    
    //Render pass
    int                 mRenderPass;
    
    //Random
    ci::vec2            mRandSeed;
    int					mRandIterations;
    
    float			mTime;				// Time elapsed in real world seconds
    float			mTimeElapsed;		// Time elapsed in simulation seconds
    float			mTimeMulti;			// Speed at which time passes
    float			mTimeAdjusted;		// Amount of time passed between last frame and current frame
    float			mTimer;				// A resetting counter for determining if a Tick has occured
    bool            mTick;

    
    //Shaders
    ci::gl::GlslProgRef		mGradientShader;
    ci::gl::GlslProgRef		mGlowShader;
    ci::gl::GlslProgRef		mNebulaShader;
    ci::gl::GlslProgRef		mDustShader;
    ci::gl::GlslProgRef		mCoronaShader;
    ci::gl::GlslProgRef		mPlanetShader;
    
    //Textures
    ci::gl::Texture2dRef	mSpectrumTex;
    ci::gl::Texture2dRef	mGlowTex;
    ci::gl::Texture2dRef	mNebulaTex;
    ci::gl::Texture2dRef	mCoronaTex;
    ci::gl::Texture2dRef	mGridTex;
    ci::gl::Texture2dRef	mSmallGridTex;
    ci::gl::Texture2dRef	mBigGlow0Tex;
    ci::gl::Texture2dRef	mBigGlow1Tex;
    ci::gl::Texture2dRef	mVenus;
};