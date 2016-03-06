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

#include "Glow.h"
#include "Planet.h"
#include "PlanetManager.h"

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
    PlanetRef           mStar;
    float               mStartColor;
    
    //Sphere
    void createSphere();
    gl::BatchRef	mSphereBatch;
    float           mRadius;
    
    
    float            mLights;
    
    //PASS
    void drawRenderPass();
    int  mRenderPass;

    
    // CONTROLLER
    void                updateStar();

    
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
    setWindowSize(1280, 720);
    
    mRenderPass = 2;
    
    mStar = Planet::create();
    
   // mStar->setRadius(50);
    mStar->setPos(ci::vec3(0));
    mStar->setMass(4000000.0f);
    
    mStar->loadShaders();
    mStar->loadTextures();
    mStar->createSphere();
    
    setupCamera();
    
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
    //mCamera.lookAt( vec3( 3, 2, 4 ), vec3( 0 ) );
    mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
    mCameraCtrl = CameraUi(&mCamera, getWindow());
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
            mStar->enableLights();
            break;
            
        case 's':
            mStar->disableLights();
            break;
            
        case 'r':
             mStar->randSeed();
            break;
            
        case 't':
            mStar->randomColor();
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
    updateStar();
}

void StarApp::updateStar()
{
    
    mStar->update();
    
    vec3 right;
    vec3 up;
    
    mCamera.getBillboardVectors(&right, &up);
    mStar->setBillboardVectors(right, up);
}


void StarApp::drawRenderPass()
{
    switch (mRenderPass) {
        case 1:
        {
            gl::enableDepthWrite();
            gl::enableDepthRead();
            
            gl::ScopedMatrices mat;
            gl::setMatrices(mCamera);
            
            mStar->drawSphere();
        }
            break;
        case 2:
            
           // gl::enable( GL_TEXTURE_2D );
           // gl::disableDepthWrite();
           // mStar->drawCircle();
        {
            gl::enableDepthWrite();
            gl::enableDepthRead();
           // gl::disableAlphaBlending();
            
            gl::setMatrices( mCamera );
            gl::ScopedModelMatrix modelScope;
            
            mStar->drawMoon(mCamera.getViewMatrix());
        }
            break;
        case 3:
            gl::enableDepthWrite();
            gl::enableDepthRead();
            gl::enableAlphaBlending();
            mStar->drawCorona();
            break;
        case 4:
            gl::disableDepthWrite();
             gl::enableAlphaBlending();
            
            gl::enable( GL_TEXTURE_2D );
            gl::color( ColorA( 1, 1, 1, 1 ) );
            mStar->drawGlows();
            
            gl::disable( GL_TEXTURE_2D );
            
            //gl::enableAdditiveBlending();
            break;
        case 5:
            gl::disableDepthWrite();
            gl::enableAlphaBlending();
            
            gl::enable( GL_TEXTURE_2D );
            gl::color( ColorA( 1, 1, 1, 1 ) );
            mStar->drawNebulas();
            
            gl::disable( GL_TEXTURE_2D );
            break;
            
        case 6:
            gl::enableAlphaBlending();
            mStar->drawDust();
            break;
            
        case 7:
        {
            gl::ScopedMatrices mat;
            gl::setMatrices(mCamera);
            
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
            
            {
                gl::setMatrices( mCamera );
                gl::ScopedModelMatrix modelScope;
                
                mStar->drawMoon(mCamera.getViewMatrix());
            }
            
            gl::disableDepthWrite();
            
            gl::enableAdditiveBlending();
                gl::color( ColorA( 1, 1, 1, 1 ) );
            
            
                {
                    gl::ScopedMatrices mat;
                    gl::rotate( mCamera.getOrientation() );
                    mStar->drawCircle();
                    mStar->drawCorona();
                }
            
                mStar->drawGlows();
                mStar->drawNebulas();
    
            gl::disable( GL_TEXTURE_2D );
            
            mStar->drawDust();

            gl::disableDepthRead();
            gl::disableDepthWrite();
            
            break;
        }
    }
}

void StarApp::draw()
{
	gl::clear( mBkgColor);

    {
        //gl::ScopedMatrices mat;
        //gl::setMatrices(mCamera);
    
        drawRenderPass();
    }
    
    mParams->draw();
    
}

CINDER_APP( StarApp, RendererGl( RendererGl::Options().msaa( 32 ) ) )
