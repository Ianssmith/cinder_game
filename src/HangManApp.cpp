#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/System.h"
#include "OscSender.h"
#include "OscListener.h"

#include "inputArea.hpp"
#include "wrongAnswers.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;



class HangManApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    void keyDown(KeyEvent event) override;

    
    inputArea   inputArea;
    wrongAnswer wrongAnswer;
    
};

void HangManApp::setup()
{
    ci::app::setWindowSize(800, 600);
    inputArea.setup();
    wrongAnswer.setup();
}

void HangManApp::mouseDown( MouseEvent event ){}

void HangManApp::update()
{
}

void HangManApp::draw()
{
	gl::clear( Color( 1, 1, 1 ) );
    inputArea.draw();
    wrongAnswer.draw();
}

void HangManApp::keyDown(KeyEvent event)
{
    if (inputArea.editable) {
        if (event.getChar() == 'a') {
            inputArea.setText("a");
        }
        if (event.getChar() == 'b') {
            inputArea.setText("b");
        }
        if (event.getChar() == 'c') {
            inputArea.setText("c");
        }
        if (event.getChar() == 'd') {
            inputArea.setText("d");
        }
        if (event.getChar() == 'e') {
            inputArea.setText("e");
        }
        if (event.getChar() == 'f') {
            inputArea.setText("f");
        }
        if (event.getChar() == 'g') {
            inputArea.setText("g");
        }
        if (event.getChar() == 'h') {
            inputArea.setText("h");
        }
        if (event.getChar() == 'i') {
            inputArea.setText("i");
        }
        if (event.getChar() == 'j') {
            inputArea.setText("j");
        }
        if (event.getChar() == 'k') {
            inputArea.setText("k");
        }
        if (event.getChar() == 'l') {
            inputArea.setText("l");
        }
        if (event.getChar() == 'm') {
            inputArea.setText("m");
        }
        if (event.getChar() == 'n') {
            inputArea.setText("n");
        }
        if (event.getChar() == 'o') {
            inputArea.setText("o");
        }
        if (event.getChar() == 'p') {
            inputArea.setText("p");
        }
        if (event.getChar() == 'q') {
            inputArea.setText("q");
        }
        if (event.getChar() == 'r') {
            inputArea.setText("r");
        }
        if (event.getChar() == 's') {
            inputArea.setText("s");
        }
        if (event.getChar() == 't') {
            inputArea.setText("t");
        }
        if (event.getChar() == 'u') {
            inputArea.setText("u");
        }
        if (event.getChar() == 'v') {
            inputArea.setText("v");
        }
        if (event.getChar() == 'w') {
            inputArea.setText("w");
        }
        if (event.getChar() == 'x') {
            inputArea.setText("x");
        }
        if (event.getChar() == 'y') {
            inputArea.setText("y");
        }
        if (event.getChar() == 'z') {
            inputArea.setText("z");
        }
        if (event.getCode() == KeyEvent::KEY_BACKSPACE) {
            inputArea.setText("");
        }
        if (event.getCode() == KeyEvent::KEY_RETURN) {
            if (inputArea.getInputText() != "") {
                inputArea.reset();
                inputArea.disableTextField();
                wrongAnswer.setWrongAnswers("f,q,d");
            }
        }
    }
}
CINDER_APP( HangManApp, RendererGl )
