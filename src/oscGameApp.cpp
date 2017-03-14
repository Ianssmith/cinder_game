#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class oscGameApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void oscGameApp::setup()
{
}

void oscGameApp::mouseDown( MouseEvent event )
{
}

void oscGameApp::update()
{
}

void oscGameApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( oscGameApp, RendererGl )
