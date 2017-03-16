#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OscListener.h"
#include "cinder/app/RendererGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HangManGuestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    void drawLine();
    void drawAnswer();
    void drawMan();
    
    int bodypart;
    char letter;
    int letterPos;
    
    osc::Listener 	listener;
    
    std::array<gl::TextureRef, 9> mBodyPart;

    
   
    gl::TextureRef mBodyTex1;
    gl::TextureRef mBodyTex2;
    gl::TextureRef mBodyTex3;
    gl::TextureRef mBodyTex4;
    gl::TextureRef mBodyTex5;
    gl::TextureRef mBodyTex6;
    gl::TextureRef mBodyTex7;
    gl::TextureRef mBodyTex8;
    gl::TextureRef mBodyTex9;
    
    
  
    
};




void HangManGuestApp::drawMan()
{
//    while( listener.hasWaitingMessages() ) {
//        osc::Message message;
//        listener.getNextMessage( &message );
//        message.addIntArg(bodypart);
//        
//    }
    
    try {
        mBodyTex1 = gl::Texture::create(loadImage(ci::app::loadAsset("body1.png")));
        mBodyTex2 = gl::Texture::create(loadImage(ci::app::loadAsset("body2.png")));
        mBodyTex3 = gl::Texture::create(loadImage(ci::app::loadAsset("body3.png")));
        mBodyTex4 = gl::Texture::create(loadImage(ci::app::loadAsset("body4.png")));
        mBodyTex5 = gl::Texture::create(loadImage(ci::app::loadAsset("body5.png")));
        mBodyTex6 = gl::Texture::create(loadImage(ci::app::loadAsset("body6.png")));
        mBodyTex7 = gl::Texture::create(loadImage(ci::app::loadAsset("body7.png")));
        mBodyTex8 = gl::Texture::create(loadImage(ci::app::loadAsset("body8.png")));
        mBodyTex9 = gl::Texture::create(loadImage(ci::app::loadAsset("body9.png")));
        
    } catch (exception& e) {
        ci::app::console() << e.what() << std::endl;
    }
    std::array<gl::TextureRef, 9> mBodyPart = {mBodyTex1, mBodyTex2, mBodyTex3, mBodyTex4, mBodyTex5, mBodyTex6, mBodyTex7, mBodyTex8, mBodyTex9};
    
    // receive int bodypart
    for (int i=0; i < bodypart; i++){
        gl::draw (mBodyPart[i],Rectf(525, 110, 675, 400));
        cout << i << endl;
    }

    
}

void HangManGuestApp::drawLine()
{
    
}

void HangManGuestApp::drawAnswer()
{
    
}


void HangManGuestApp::setup()
{
    setWindowSize(800, 600);
    
}



void HangManGuestApp::mouseDown( MouseEvent event )
{
}

void HangManGuestApp::update()
{
}

void HangManGuestApp::draw()
{
	gl::clear( Color( 1, 1, 1 ) );
    
    //draw hanger
    gl::color(ci::Color(0.5F, 0.5F, 0.5F));
    gl::drawSolidRect(Rectf(600, 0, 607, 110));
    
    //draw stand
    gl::color(ci::Color(0.6f,0.5f, 0.4f));
    gl::drawSolidRect(Rectf(480, 440, 800, 600));
    
    drawMan();

}

CINDER_APP( HangManGuestApp, RendererGl )
