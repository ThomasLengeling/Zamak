#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Arcball.h"
#include "cinder/CameraUi.h"
#include "cinder/params/Params.h"

#include "ParticleManager.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class ParticleSphereApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    
    ParticleManagerRef mParticleManager;
    
    //CAMERA
    CameraPersp					mCamera;
    CameraUi					mCameraCtrl;
    void                        setupCamera();

};

void ParticleSphereApp::setup()
{
    
    setWindowSize(ci::ivec2(1280, 720));
    mParticleManager = ParticleManager::create();
    
    //create Particles
    int numParticles = 20;
    for(int i = 0; i < numParticles; i++){
        Particle * p = new Particle();
        float rad = 1.0f;
        
        float theta = ci::randFloat(0, M_PI);
        float phi = ci::randFloat(0, 2 * M_PI);
        
        float x = rad * sin( theta ) * cos( phi );
        float y = rad * sin( theta ) * sin( phi );
        float z = rad * cos( theta );
        
        ci::vec3 pos = ci::vec3(x, y, z);
        p->setPosition(pos);
        mParticleManager->addParticle(p);
    }
    
    setupCamera();
}

void ParticleSphereApp::setupCamera()
{
    //camera
    float verticalFOV = 65.0f;
    float aspectRatio = getWindowAspectRatio();
    float nearClip = 1.0f;
    float farClip = 200.0f;
    
    vec3 cameraPosition = vec3(0, 0, 5);
    vec3 cameraTarget   = vec3(0, 0, 0);
    vec3 cameraUpAxis   = vec3(0, 1, 0);
    
    mCamera.setPerspective(verticalFOV, aspectRatio, nearClip, farClip);
    mCamera.lookAt(cameraPosition, cameraTarget, cameraUpAxis);
    mCameraCtrl = CameraUi(&mCamera, getWindow());
}

void ParticleSphereApp::mouseDown( MouseEvent event )
{
    
}

void ParticleSphereApp::update()
{
    mParticleManager->update();
}

void ParticleSphereApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    
    {
        gl::enableDepthRead();
        gl::enableDepthWrite();
        gl::ScopedMatrices mat;
        gl::setMatrices(mCamera);
        mParticleManager->draw();
        
        //gl::enableWireframe();
        //gl::drawSphere(ci::vec3(0), 1, 16);
        //gl::disableWireframe();
        
    
        mParticleManager->drawPathBetweenVectors();
        
        
    }
}

CINDER_APP( ParticleSphereApp, RendererGl( RendererGl::Options().msaa( 32 ) ) )
