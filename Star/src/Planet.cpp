//
//  Planet.cpp
//  Star
//
//  Created by tom on 2/22/16.
//
//

#include "Planet.h"

#define M_4_PI 12.566370614359172

using namespace std;
using namespace ci;

Planet::Planet()
{
    mColor		= ci::Rand::randFloat();
    mAngle		= ci::Rand::randFloat( M_PI * 2.0f );
    
    mColorDest      = 0.6f;
    mColor          = mColorDest;
    mMaxRadius      = 200.0f;
    
    mRandIterations = 24704;
    
    //time
    mTime			= (float)app::getElapsedSeconds();
    mTimeElapsed	= 0.0f;
    mTimeMulti		= 60.0f;
    mTimer			= 0.0f;
    mTick			= false;
    
    randSeed();
    
    mCircleBrightness = 1.0;
}

void Planet::setMass(float mass)
{
    mMass = mass;
    mRadiusDest = powf( (3.0f * mMass )/(float)M_4_PI, 0.3333333f );
    mRadius		= mRadiusDest * 0.5f;
}

void Planet::setOrbits(float orbitRadius, float orbitSpeed, float radius)
{
    mOrbitRadius	= orbitRadius;
    mOrbitSpeed		= orbitSpeed;
    mOrbitRadius    = radius;
    
}

void Planet::update()
{
    updateRotation(mTimeAdjusted);
    updateTime();
    updateParticles();
}

void Planet::updateRotation( float dt )
{
    mAngle	   += mOrbitSpeed * dt;
    float cosA	= cos( mAngle );
    float sinA	= sin( mAngle );
    mOrbitPos   = ci::vec3( cosA, 0.0f, sinA ) * mOrbitRadius;
}

void Planet::updateTime()
{
    float prevTime	= mTime;
    mTime			= (float)app::getElapsedSeconds();
    float dt		= mTime - prevTime;
    
    mTimeAdjusted	= dt * mTimeMulti;
    mTimeElapsed   += mTimeAdjusted;
    
    mTimer += mTimeAdjusted;

    mTick = false;
    if( mTimer > 1.0f ){
        mTick = true;
        mTimer = 0.0f;
    }
    
}

void Planet::updateParticles()
{
    
    mRadius		-= ( mRadius - mRadiusDest ) * 0.1f;
    mRadiusDrawn = mRadius * 0.9f;
    float radiusPer = mRadius/mMaxRadius;
    mRadiusMulti = lerp( 0.2f, 4.5f, radiusPer * radiusPer );
    mColor		-= ( mColor - mColorDest ) * 0.1;
    
    int numGlowsToSpawn = 36;
    mController.addGlows( mPos, mRadius, mRadiusMulti, mLights, numGlowsToSpawn );
    
    // ADD NEBULAS
    int numNebulasToSpawn = 36;
    mController.addNebulas( mPos, mRadius, mRadiusMulti, numNebulasToSpawn );
    
    // ADD DUSTS
    int numDustsToSpawn = 500;
    mController.addDusts( mPos, mVel, mRadius, numDustsToSpawn );
    
    mController.update( mTimeAdjusted );
}

void Planet::draw()
{
    ci::gl::drawSphere( mPos, mRadius, 32 );
}


// SHADERS  and   TEXTURES
void Planet::loadTextures()
{
    auto fmt = gl::Texture2d::Format().wrap(GL_REPEAT ).mipmap().minFilter( GL_LINEAR_MIPMAP_LINEAR );
    
    mSpectrumTex		= gl::Texture2d::create( loadImage( ci::app::loadAsset( "spectrum.png" ) ) );
    mGlowTex			= gl::Texture2d::create( loadImage( ci::app::loadAsset( "glow.png" ) ) );
    mNebulaTex			= gl::Texture2d::create( loadImage( ci::app::loadAsset( "nebula.png" ) ) );
    mCoronaTex			= gl::Texture2d::create( loadImage( ci::app::loadAsset( "corona.png" ) ) );
    mGridTex			= gl::Texture2d::create( loadImage( ci::app::loadAsset( "grid.png" ) ) );
    mSmallGridTex		= gl::Texture2d::create( loadImage( ci::app::loadAsset( "smallGrid.png" ) ) );
    mBigGlow0Tex		= gl::Texture2d::create( loadImage( ci::app::loadAsset( "bigGlow0.png" ) ) );
    mBigGlow1Tex		= gl::Texture2d::create( loadImage( ci::app::loadAsset( "bigGlow1.png" ) ) );
    mVenus              = gl::Texture2d::create( ci::loadImage( ci::app::loadAsset( "venus.jpg" ) ), fmt );
    
 //   mMoonTex            = gl::Texture2d::create( ci::loadImage( ci::app::loadAsset( "moon.jpg" ) ), gl::Texture::Format().mipmap() );
 //   mMoonBMTex          = gl::Texture2d::create( ci::loadImage( ci::app::loadAsset( "normal.jpg" ) ), gl::Texture::Format().mipmap() );
}

void Planet::loadShaders()
{
    // LOAD SHADERS
    try {
        mGradientShader = gl::GlslProg::create( ci::app::loadAsset( "passThru.vert" ), ci::app::loadAsset( "gradient.frag" ) );
        mGlowShader		= gl::GlslProg::create( ci::app::loadAsset( "passThru.vert" ), ci::app::loadAsset( "glow.frag" ) );
        mNebulaShader	= gl::GlslProg::create( ci::app::loadAsset( "passThru.vert" ), ci::app::loadAsset( "nebula.frag" ) );
        mCoronaShader	= gl::GlslProg::create( ci::app::loadAsset( "passThru.vert" ), ci::app::loadAsset( "corona.frag" ) );
        mDustShader		= gl::GlslProg::create( ci::app::loadAsset( "passThruColor.vert" ), ci::app::loadAsset( "dust.frag" ) );
        
        
        mTBNShader = gl::GlslProg::create( ci::app::loadAsset( "tbn.vert" ), ci::app::loadAsset( "tbn.frag" ) );
        //mPlanetShader	= gl::GlslProg::create( loadAsset( "passThruNormals.vert" ), loadAsset( "planet.frag" ) );
    } catch( gl::GlslProgCompileExc e ) {
        std::cout << e.what() << std::endl;
        //quit();
    }
    
    
    mMoonTex =  gl::Texture2d::create( ci::loadImage( ci::app::loadAsset( "moon.jpg" ) ), gl::Texture::Format().mipmap() );
    mMoonTex->bind();
    mMoonBMTex = gl::Texture2d::create( ci::loadImage( ci::app::loadAsset( "normal.jpg" ) ), gl::Texture::Format().mipmap() );
    mMoonBMTex->bind( 1 );


    mMoonBatch = gl::Batch::create( geom::Sphere().subdivisions(64) >> geom::Transform( scale( vec3(mRadius*1.6 ) ) ), mTBNShader );/// gl::Batch::create( geom::Sphere().radius(mRadius).subdivisions(64), mTBNShader );
    gl::ScopedGlslProg glslScp( mTBNShader );
    mTBNShader->uniform( "uDiffuseMap", 0 );
    mTBNShader->uniform( "uNormalMap", 1 );
    mTBNShader->uniform( "uLightLocViewSpace", vec3( 0, 0, 1 ) );
}

void Planet::createSphere()
{
    
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef shader = gl::getStockShader( lambert );
    
   // float solarRadius = mMaxRadius * 0.25f;
    
   // mRadius = solarRadius;
    mStartColor = 0.95f;
    
    mCoronaColor = Rand::randFloat( 0, 1.0f );
    mDustColor   = Rand::randFloat( 0, 1.0f );
    mGlowColor   = Rand::randFloat( 0, 1.0f );
    mCircleColor = Rand::randFloat( 0, 1.0f );
    mNebulaColor = Rand::randFloat( 0, 1.0f );
    
    ci::app::console()<<"solar radius: "<<mRadius<<std::endl;
    
    //mRadius = 0.1f;
    int sphereResolution = 32;
    mSphereBatch = gl::Batch::create(geom::Sphere().radius(mRadius*1.7).subdivisions(sphereResolution), shader );
    
    mLights = 1.0;
}


void Planet::drawCorona()
{
    gl::color( ColorA( 1, 1, 1, 1 ) );
    float radius = mRadius * 1.8f;
    
    gl::ScopedGlslProg scpGlsl(mCoronaShader);
    gl::ScopedTextureBind scpCorina(mCoronaTex, 0);
    gl::ScopedTextureBind scpSpectrum(mCoronaTex, 1);
    
    mCoronaShader->uniform( "coronaTex", 0 );
    mCoronaShader->uniform( "spectrumTex", 1 );
    mCoronaShader->uniform( "starColor", mCoronaColor );
    mCoronaShader->uniform( "power",  mLights );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
}

void Planet::drawGlows()
{
    {
        gl::ScopedGlslProg scpGlsl(mGlowShader);
        gl::ScopedTextureBind scpGlow(mGlowTex, 0);
        gl::ScopedTextureBind scpSmallGrid(mSmallGridTex, 1);
        gl::ScopedTextureBind scpSpectrumTex(mSpectrumTex, 2);
        
        mGlowShader->uniform( "glowTex", 0 );
        mGlowShader->uniform( "gridTex", 1 );
        mGlowShader->uniform( "spectrumTex", 2 );
        mGlowShader->uniform( "color", mGlowColor );
        mGlowShader->uniform( "power", mLights );
        mGlowShader->uniform( "starRadius", mRadiusDrawn );
        
        mController.drawGlows( mGlowShader, mRight, mUp );
    }
}

void Planet::drawNebulas()
{
    {
        gl::ScopedGlslProg scpGlsl(mNebulaShader);
        
        gl::ScopedTextureBind scpGlow(mNebulaTex, 0);
        gl::ScopedTextureBind scpSmallGrid(mSmallGridTex, 1);
        gl::ScopedTextureBind scpSpectrumTex(mSpectrumTex, 2);
        
        mNebulaShader->uniform( "nebulaTex", 0 );
        mNebulaShader->uniform( "gridTex", 1 );
        mNebulaShader->uniform( "spectrumTex", 2 );
        mNebulaShader->uniform( "color", mNebulaColor );
        mNebulaShader->uniform( "power", mLights );
        mNebulaShader->uniform( "starRadius", mRadiusDrawn/2.0f );
        
        mController.drawNebulas( mNebulaShader, mRight, mUp );
    }
}

void Planet::drawSphere()
{
    
    mSphereBatch->draw();
}

void Planet::drawDust()
{
    float per = 1.75f * mRadius/mMaxRadius;
    gl::scale( vec3( per, per, per ) );
    
    gl::ScopedGlslProg scpGlsl(mDustShader);
    gl::ScopedTextureBind scpSpectrum(mSpectrumTex, 0);
    gl::ScopedModelMatrix model;
    
    mDustShader->uniform( "spectrumTex", 0 );
    mDustShader->uniform( "color", mDustColor );
    mDustShader->uniform( "power", mLights );
    mController.drawDusts();
}

void Planet::drawMoon(ci::mat4 camViewMatrix)
{
   gl::color( ColorA( 1, 1, 1, 1 ) );
   float radius = mRadius;
   radius *= ( 5.0f + mLights );
    

    ci::vec3 mLightPosWorldSpace = vec3( mRadius, mRadius, mRadius );
    vec3 light = vec3( camViewMatrix * vec4( mLightPosWorldSpace, 1 )) ;
    //ci::app::console()<<light<<std::endl;
    mTBNShader->uniform( "uLightLocViewSpace", light);
    mTBNShader->uniform(  "time", (float)ci::app::getElapsedSeconds() );
    mMoonBatch->draw();
    
}

void Planet::drawCircle()
{
    gl::color( ColorA( 1, 1, 1, 1 ) );
    float radius = mRadius;
    radius *= ( 5.0f + mLights );
    
    gl::ScopedGlslProg scpGlsl(mGradientShader);
    gl::ScopedTextureBind scpGrid(mGridTex, 0);
    gl::ScopedTextureBind scpSpectrum(mSpectrumTex, 1);
    gl::ScopedTextureBind scpVenus(mVenus, 2);
    
    mGradientShader->uniform( "gridTex", 0 );
    mGradientShader->uniform( "spectrumTex", 1 );
    mGradientShader->uniform( "venusTex", 2 );
    
    mGradientShader->uniform( "color", mCircleColor );
    mGradientShader->uniform( "radius", radius );
    mGradientShader->uniform( "starRadius", mRadiusDrawn );
    mGradientShader->uniform( "power", mLights );
    mGradientShader->uniform( "time", (float)ci::app::getElapsedSeconds() );
    mGradientShader->uniform( "randIterations", mRandIterations );
    mGradientShader->uniform( "brightness", mCircleBrightness);
    
    mGradientShader->uniform( "randSeed", mRandSeed );
    mGradientShader->uniform( "unit", 0 );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
    
    mGradientShader->uniform( "randSeed", mRandSeed * 1.5f );
    mGradientShader->uniform( "unit", 2 );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
    
   // ci::app::console()<<radius<<" "<<mRandIterations<<" "<<mRadiusDrawn<<" "<<mRadius<<std::endl;
}

void Planet::randSeed()
{
    mRandSeed	= Rand::randVec2() * Rand::randFloat( 100.0f );
    mRandIterations ++;
}
