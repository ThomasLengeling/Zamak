#include "ZamakEmApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;


void ZamakEMApp::setup()
{
    setWindowSize(ci::ivec2(1024, 768));
    
    // this will create the renderer and initialize the ui
    // resources. Pass a ui::Options() argument if you want
    // to change the default settings
    ui::initialize();
    
    zamak::Event event = makeSabreEvent("inicial");
    mEvents.push_back(event);
}

void ZamakEMApp::mouseDown( MouseEvent event )
{
}

void ZamakEMApp::update()
{
    updateEvents();
}

void ZamakEMApp::updateEvents()
{
    
    // Our List / Object selector
    static const zamak::Event * selection =  &mEvents[0];
    {
        ui::ScopedWindow window( "Objects" );
        
        // add / remove buttons
        if( ui::Button( "Add" ) ) {
            static int objCount = mEvents.size();
            zamak::Event event = makeSabreEvent("Evento "+to_string( objCount++ ));
            
            mEvents.push_back(event);
        }
        if( selection ) {
            ui::SameLine();
            if( ui::Button( "Remove" ) ) {
                auto it = std::find_if( mEvents.begin(), mEvents.end(), [] ( const zamak::Event & obj ) { return & obj == selection; } );
                if( it != mEvents.end() ) {
                    mEvents.erase( it );
                }
            }
        }
        
        // selectable list
        ui::ListBoxHeader( "" );
        for( const auto & object : mEvents ) {
            if( ui::Selectable( object.mEventName.c_str(), selection == &object ) ) {
                selection = &object;
            }
        }
        ui::ListBoxFooter();
    }
    
    // The Object Inspector
    if( selection ) {
        ui::ScopedWindow window( "Toggles" );
        
        zamak::Event * evento = (zamak::Event *)selection;
        
        ui::InputText("Evento: ", &evento->mEventName);
        
        //mAccelero
        ui::Checkbox(std::get<0>(evento->mSabreStream.mAccelero).c_str(), &std::get<1>(evento->mSabreStream.mAccelero));
        ui::InputFloat3("Value", &std::get<2>(evento->mSabreStream.mAccelero)[0]);
        
        //mGyro
        ui::Checkbox(std::get<0>(evento->mSabreStream.mGyro).c_str(), &std::get<1>(evento->mSabreStream.mGyro));
        ui::InputFloat3("Value", &std::get<2>(evento->mSabreStream.mGyro)[0]);
        
        //Magneto
        ui::Checkbox(std::get<0>(evento->mSabreStream.mMagneto).c_str(), &std::get<1>(evento->mSabreStream.mMagneto));
        ui::InputFloat3("Value", &std::get<2>(evento->mSabreStream.mMagneto)[0]);
        
        //Heading
        ui::Checkbox(std::get<0>(evento->mSabreStream.mHeading).c_str(), &std::get<1>(evento->mSabreStream.mHeading));
        ui::InputFloat("Value", &std::get<2>(evento->mSabreStream.mHeading));
        
        //Tilt
        ui::Checkbox(std::get<0>(evento->mSabreStream.mTilt).c_str(), &std::get<1>(evento->mSabreStream.mTilt));
        ui::InputFloat("Value", &std::get<2>(evento->mSabreStream.mTilt));
        
        //Buttons
        ui::Checkbox(std::get<0>(evento->mSabreStream.mBottons).c_str(), &std::get<1>(evento->mSabreStream.mBottons));
        ui::InputInt3("Value", &std::get<2>(evento->mSabreStream.mBottons)[0]);
        
        //Pressure
        ui::Checkbox(std::get<0>(evento->mSabreStream.mPressure).c_str(), &std::get<1>(evento->mSabreStream.mPressure));
        ui::InputFloat("Value", &std::get<2>(evento->mSabreStream.mPressure));
        
        //KeyCode
        ui::Checkbox(std::get<0>(evento->mSabreStream.mKeyCode).c_str(), &std::get<1>(evento->mSabreStream.mKeyCode));
        ui::InputFloat("Value", &std::get<2>(evento->mSabreStream.mKeyCode));
        
        //Node
        ui::Checkbox(std::get<0>(evento->mSabreStream.mNode).c_str(), &std::get<1>(evento->mSabreStream.mNode));
        ui::InputFloat("Value", &std::get<2>(evento->mSabreStream.mNode));
    }
}


/*
 std::tuple< std::string, bool, ci::vec3 >              mAccelero;
 std::tuple< std::string, bool, ci::vec3 >              mGyro;
 std::tuple< std::string, bool, ci::vec3 >              mMagneto;
 std::tuple< std::string, bool, float >                 mHeading;
 std::tuple< std::string, bool, float >                 mTilt;
 std::tuple< std::string, bool, ci::ivec3 >             mBottons;
 std::tuple< std::string, bool, float >                 mPressure;
 std::tuple< std::string, bool, float >                 mKeyCode;
 std::tuple< std::string, bool, float >                 mNode;
 */

zamak::Event ZamakEMApp::makeSabreEvent(std::string name)
{
    zamak::Event event;
    
    event.mEventName = name;
    
    //keys
    for(int i = 0; i < sabre::MAX_KEYS; i++){
        event.mSabreStream.mKeys.push_back(std::make_tuple("Keys_"+to_string(i), true, 0.1f));
    }
    
    event.mSabreStream.mGyro = std::make_tuple("Gyro", true, ci::vec3(0.1f));
    event.mSabreStream.mMagneto = std::make_tuple("Magneto", true, ci::vec3(0.1f));
    event.mSabreStream.mHeading = std::make_tuple("Heading", true, 0.1f);
    event.mSabreStream.mTilt = std::make_tuple("Tilt", true, 0.1f);
    event.mSabreStream.mBottons = std::make_tuple("Buttons", true, ci::ivec3(0.1f));
    event.mSabreStream.mPressure= std::make_tuple("Pressure", true, 0.1f);
    event.mSabreStream.mKeyCode = std::make_tuple("Key", true, 0.1f);
    event.mSabreStream.mNode = std::make_tuple("Node", true, 0.1f);
    event.mSabreStream.mAccelero = std::make_tuple("accelero", true, ci::vec3(0.1f));
    //
    return event;
}

void ZamakEMApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
}

CINDER_APP( ZamakEMApp, RendererGl )
