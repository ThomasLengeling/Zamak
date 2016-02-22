#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Arcball.h"
#include "cinder/CameraUi.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "Controller.h"
#include "Glow.h"
#include "Star.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class StarApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown( KeyEvent event) override;
	void update() override;
	void draw() override;
    
    //CAMERA
    CameraPersp					mCamera;
    CameraUi					mCameraCtrl;
    void                        setupCamera();
    
    //STAR
    Star				mStar;
    float               mStartColor;
    
    //Sphere
    void createSphere();
    gl::BatchRef	mSphereBatch;
    float           mRadius;
    
    
    float            mLights;
    
    //PASS
    void drawRenderPass();
    int  mRenderPass;
    
    void drawCorona();
    void drawSphere();
    void drawGlows();
    void drawNebulas();
    void drawDust();
    void drawCircle();
    
    //Random
    vec2				mRandSeed;
    int					mRandIterations;
    void                randSeed();
    
    // SHADERS  and   TEXTURES
    void loadTextures();
    void loadShaders();

    gl::GlslProgRef		mGradientShader;
    gl::GlslProgRef		mGlowShader;
    gl::GlslProgRef		mNebulaShader;
    gl::GlslProgRef		mDustShader;
    gl::GlslProgRef		mCoronaShader;
    gl::GlslProgRef		mPlanetShader;
    
    gl::Texture2dRef	mSpectrumTex;
    gl::Texture2dRef	mGlowTex;
    gl::Texture2dRef	mNebulaTex;
    gl::Texture2dRef	mCoronaTex;
    gl::Texture2dRef	mGridTex;
    gl::Texture2dRef	mSmallGridTex;
    gl::Texture2dRef	mBigGlow0Tex;
    gl::Texture2dRef	mBigGlow1Tex;
    gl::Texture2dRef	mVenus;
    
    // CONTROLLER
    Controller			mController;
    void                updateParticles();
    
    //TIMERS
    void setupTime();
    void updateTime();
    float			mTime;				// Time elapsed in real world seconds
    float			mTimeElapsed;		// Time elapsed in simulation seconds
    float			mTimeMulti;			// Speed at which time passes
    float			mTimeAdjusted;		// Amount of time passed between last frame and current frame
    float			mTimer;				// A resetting counter for determining if a Tick has occured
    bool            mTick;
    
    //Params
    params::InterfaceGlRef		mParams;
    float                       mCircleBrightness;
    float                       mCoronaColor;
    float                       mDustColor;
    float                       mGlowColor;
    float                       mCircleColor;
    float                       mNebulaColor;
    
    ci::Color                   mBkgColor;
};

void StarApp::setup()
{
    setWindowSize(1920, 1080);
    
    mRenderPass = 7;
    
    loadShaders();
    loadTextures();
    
    createSphere();
    
    setupCamera();
    
    setupTime();
    
    mCircleBrightness = 1.0;
    mParams = params::InterfaceGl::create( "Start", ivec2( 250, 250 ) );
    mParams->addParam("Bkg", &mBkgColor);
    mParams->addParam("Brightness", &mCircleBrightness).min(0.0).max(1.0).step(0.01);
    mParams->addParam("Corona Color", &mCoronaColor).min(0.0).max(1.0).step(0.01);
    mParams->addParam("Dust Color", &mDustColor).min(0.0).max(1.0).step(0.01);
    mParams->addParam("Glow Color", &mGlowColor).min(0.0).max(1.0).step(0.01);
    mParams->addParam("Circle Color", &mCircleColor).min(0.0).max(1.0).step(0.01);
    mParams->addParam("Nebula Color", &mNebulaColor).min(0.0).max(1.0).step(0.01);
    
    mBkgColor = ci::Color(0, 0, 0);
    
}

void StarApp::setupTime()
{
    // TIME
    mTime			= (float)app::getElapsedSeconds();
    mTimeElapsed	= 0.0f;
    mTimeMulti		= 60.0f;
    mTimer			= 0.0f;
    mTick			= false;
}

void StarApp::createSphere()
{
    
    mStar				= Star( vec3(0), 4000000.0f );
    
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef shader = gl::getStockShader( lambert );
    
    float solarRadius = mStar.mMaxRadius * 0.25f;
    
    mRadius = solarRadius;
    mStartColor = 0.95f;
    
    mCoronaColor = mStartColor;
    mDustColor   = mStartColor;
    mGlowColor   = mStartColor;
    mCircleColor = mStartColor;
    mNebulaColor = mStartColor;
    
    console()<<"solar radius: "<<solarRadius<<std::endl;
    
    //mRadius = 0.1f;
    int sphereResolution = 32;
    mSphereBatch = gl::Batch::create(geom::Sphere().radius(solarRadius).subdivisions(sphereResolution), shader );
    
    mLights = 1.0;
}

void StarApp::setupCamera()
{
    float verticalFOV = 65.0f;
    float aspectRatio = getWindowAspectRatio();
    float nearClip = 5.0f;
    float farClip = 30000.0f;
    vec3 cameraPosition = vec3(0, 0, -366.0);
    vec3 cameraTarget = vec3(0, 0, 0);
    vec3 cameraUpAxis = vec3(0, 1, 0);
    
    mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
    mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
    mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void StarApp::loadShaders()
{
    // LOAD SHADERS
    try {
        mGradientShader = gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "gradient.frag" ) );
        mGlowShader		= gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "glow.frag" ) );
        mNebulaShader	= gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "nebula.frag" ) );
        mCoronaShader	= gl::GlslProg::create( loadAsset( "passThru.vert" ), loadAsset( "corona.frag" ) );
        mDustShader		= gl::GlslProg::create( loadAsset( "passThruColor.vert" ), loadAsset( "dust.frag" ) );
        //mPlanetShader	= gl::GlslProg::create( loadAsset( "passThruNormals.vert" ), loadAsset( "planet.frag" ) );
    } catch( gl::GlslProgCompileExc e ) {
        std::cout << e.what() << std::endl;
        //quit();
    }
}


void StarApp::loadTextures()
{
    auto fmt = gl::Texture2d::Format().wrap(GL_REPEAT ).mipmap().minFilter( GL_LINEAR_MIPMAP_LINEAR );
    
    mSpectrumTex		= gl::Texture2d::create( loadImage( loadAsset( "spectrum.png" ) ) );
    mGlowTex			= gl::Texture2d::create( loadImage( loadAsset( "glow.png" ) ) );
    mNebulaTex			= gl::Texture2d::create( loadImage( loadAsset( "nebula.png" ) ) );
    mCoronaTex			= gl::Texture2d::create( loadImage( loadAsset( "corona.png" ) ) );
    mGridTex			= gl::Texture2d::create( loadImage( loadAsset( "grid.png" ) ) );
    mSmallGridTex		= gl::Texture2d::create( loadImage( loadAsset( "smallGrid.png" ) ) );
    mBigGlow0Tex		= gl::Texture2d::create( loadImage( loadAsset( "bigGlow0.png" ) ) );
    mBigGlow1Tex		= gl::Texture2d::create( loadImage( loadAsset( "bigGlow1.png" ) ) );
    mVenus			= gl::Texture2d::create( loadImage( loadAsset( "venus.jpg" ) ), fmt );
}

void StarApp::mouseDown( MouseEvent event )
{
    
}

void StarApp::keyDown(KeyEvent event)
{
    switch (event.getChar()) {
        case '1':
            mRenderPass = 1;
             console()<<mRenderPass<<std::endl;
            break;
        case '2':
            mRenderPass = 2;
            console()<<mRenderPass<<std::endl;
            break;
        case '3':
            mRenderPass = 3;
             console()<<mRenderPass<<std::endl;
            break;
        case '4':
            mRenderPass = 4;
            break;
        case '5':
            mRenderPass = 5;
            break;
        case '6':
            mRenderPass = 6;
            break;
        case '7':
            mRenderPass = 7;
            break;
        case 'a':
            mLights = 0.0;
            break;
        case 's':
            mLights = 1.0;
            break;
        case 'r':
            randSeed();
            mRandIterations ++;
            break;
        case 't':
            mStartColor = ci::randFloat(0, 1.0);
            
            
            mCoronaColor = mStartColor;
            mDustColor = mStartColor;
            mGlowColor = mStartColor;
            mCircleColor = mStartColor;
            mNebulaColor = mStartColor;
            
            break;
        case 'q':
            mCircleBrightness += 0.01;
            break;
        case 'w':
            mCircleBrightness -= 0.01;
            break;
        default:
            break;
    }
}

void StarApp::update()
{
    updateParticles();
}

void StarApp::updateParticles()
{
    
    updateTime();
    
    mStar.update( mTimeAdjusted );
    
    int numGlowsToSpawn = 36;
    mController.addGlows( mStar, mLights, numGlowsToSpawn );
    
    // ADD NEBULAS
    int numNebulasToSpawn = 36;
    mController.addNebulas( mStar, numNebulasToSpawn );
    
    // ADD DUSTS
    int numDustsToSpawn = 500;
    mController.addDusts( mStar, numDustsToSpawn );
    
    mController.update( mTimeAdjusted );

}

void StarApp::drawCorona()
{
    gl::color( ColorA( 1, 1, 1, 1 ) );
    float radius = mStar.mRadius * 1.8f;
    
    gl::ScopedGlslProg scpGlsl(mCoronaShader);
    gl::ScopedTextureBind scpCorina(mCoronaTex, 0);
    gl::ScopedTextureBind scpSpectrum(mCoronaTex, 1);
    
    mCoronaShader->uniform( "coronaTex", 0 );
    mCoronaShader->uniform( "spectrumTex", 1 );
    mCoronaShader->uniform( "starColor", mCoronaColor );
    mCoronaShader->uniform( "power",  mLights );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
}

void StarApp::drawGlows()
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
        mGlowShader->uniform( "starRadius", mStar.mRadiusDrawn );
        vec3 right = vec3(1, 0, 0);
        vec3 up	= vec3(0, 1, 0);
        
        mCamera.getBillboardVectors(&right, &up);
        mController.drawGlows( mGlowShader, right, up );
    }
}

void StarApp::drawNebulas()
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
        mNebulaShader->uniform( "starRadius", mStar.mRadiusDrawn );
        vec3 right = vec3(1, 0, 0);
        vec3 up	= vec3(0, 1, 0);
    
        mCamera.getBillboardVectors(&right, &up);
        mController.drawNebulas( mNebulaShader, right, up );
    }
}

void StarApp::drawSphere()
{
    gl::ScopedMatrices mat;
    gl::setMatrices(mCamera);
    
   // gl::ScopedDepth depth(true, true);
    
    mSphereBatch->draw();
}

void StarApp::drawDust()
{
    gl::pushModelView();
    float per = 1.75f * mStar.mRadius/mStar.mMaxRadius;
    gl::scale( vec3( per, per, per ) );
    
    
    gl::ScopedGlslProg scpGlsl(mDustShader);
    gl::ScopedTextureBind scpSpectrum(mSpectrumTex, 0);
    
    mDustShader->uniform( "spectrumTex", 0 );
    mDustShader->uniform( "color", mDustColor );
    mDustShader->uniform( "power", mLights );
    mController.drawDusts();
    
    gl::popModelView();
}

void StarApp::drawCircle()
{
    gl::color( ColorA( 1, 1, 1, 1 ) );
    float radius = mStar.mRadius;
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
    mGradientShader->uniform( "starRadius", mStar.mRadiusDrawn );
    mGradientShader->uniform( "power", mLights );
    mGradientShader->uniform( "time", (float)getElapsedSeconds() );
    mGradientShader->uniform( "randIterations", mRandIterations );
    mGradientShader->uniform( "brightness", mCircleBrightness);
    
    mGradientShader->uniform( "randSeed", mRandSeed );
    mGradientShader->uniform( "unit", 0 );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
    
    mGradientShader->uniform( "randSeed", mRandSeed * 1.5f );
    mGradientShader->uniform( "unit", 2 );
    gl::drawSolidRect( Rectf( -radius, -radius, radius, radius ) );
    
}

void StarApp::randSeed()
{
    mRandSeed	= Rand::randVec2() * Rand::randFloat( 100.0f );
}

void StarApp::drawRenderPass()
{
    switch (mRenderPass) {
        case 1:
            gl::enableDepthWrite();
            gl::enableDepthRead();
            drawSphere();
            break;
        case 2:
            
            gl::enable( GL_TEXTURE_2D );
            gl::disableDepthWrite();
            
            drawCircle();
            break;
        case 3:
            gl::enableDepthWrite();
            gl::enableDepthRead();
            gl::enableAlphaBlending();
            drawCorona();
            break;
        case 4:
            gl::disableDepthWrite();
             gl::enableAlphaBlending();
            
            gl::enable( GL_TEXTURE_2D );
            gl::color( ColorA( 1, 1, 1, 1 ) );
            drawGlows();
            
            gl::disable( GL_TEXTURE_2D );
            
            //gl::enableAdditiveBlending();
            break;
        case 5:
            gl::disableDepthWrite();
            gl::enableAlphaBlending();
            
            gl::enable( GL_TEXTURE_2D );
            gl::color( ColorA( 1, 1, 1, 1 ) );
            drawNebulas();
            
            gl::disable( GL_TEXTURE_2D );
            break;
            
        case 6:
            gl::enableAlphaBlending();
            drawDust();
            break;
            
        case 7:
           	gl::disableDepthRead();
            gl::disableDepthWrite();
            
            gl::enableAlphaBlending();
            
            gl::enable( GL_TEXTURE_2D );
            gl::enableDepthRead();
            gl::enableDepthWrite();
            
            gl::color(0, 0, 0);
            float xpos = cos(getElapsedSeconds() * 0.04) * 100;
            float ypos = sin(getElapsedSeconds() * 0.04) * 100;
            gl::drawSphere(ci::vec3(xpos, -30, ypos), 1, 32);
            
            gl::disableDepthWrite();
            
            gl::enableAdditiveBlending();
                gl::color( ColorA( 1, 1, 1, 1 ) );
            
            
                {
                    gl::ScopedMatrices mat;
                    gl::rotate( mCamera.getOrientation() );
                    drawCircle();
                    drawCorona();
                }
            
                drawGlows();
                drawNebulas();
    
            gl::disable( GL_TEXTURE_2D );
            
            drawDust();

            gl::disableDepthRead();
            gl::disableDepthWrite();
            
            break;
    }
}

void StarApp::draw()
{
	gl::clear( mBkgColor);

    {
        gl::ScopedMatrices mat;
        gl::setMatrices(mCamera);
    
        drawRenderPass();
    }
    
    //mParams->draw();
    
}

void StarApp::updateTime()
{
    float prevTime	= mTime;
    mTime			= (float)app::getElapsedSeconds();
    float dt		= mTime - prevTime;
    mTimeAdjusted	= dt * mTimeMulti;
    mTimeElapsed	+= mTimeAdjusted;
    
    mTimer += mTimeAdjusted;
    
    //console()<<mTime<<" "<<mTimer<<" "<<mTimeElapsed<<" "<<getElapsedFrames()<<" "<<getElapsedSeconds()<< std::endl;
    
    mTick = false;
    if( mTimer > 1.0f ){
        mTick = true;
        mTimer = 0.0f;
    }
    
    //console()<<mTimeAdjusted<<std::endl;
}

CINDER_APP( StarApp, RendererGl( RendererGl::Options().msaa( 32 ) ) )
