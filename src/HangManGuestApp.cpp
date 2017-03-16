#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OscListener.h"
#include "OscSender.h"
#include "cinder/app/RendererGl.h"
#include "wrongAnswers.hpp"
#include "inputArea.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class HangManGuestApp : public App {
  public:
	void setup() override;
    void keyDown(KeyEvent event) override;
    void mouseDown(MouseEvent   event) override;
	void update() override;
	void draw() override;
    
    void drawLine();
    void drawAnswer();
    void drawMan();
    
    string modifyAnswer(string answer);
    
    osc::Listener 	listener; //recieve game data
    osc::Sender     sender; //send input and player ID
    std::string     host;// when you are a sender, host is the judger
    bool    bActivated;
    int player;
    int answerLength;
    //string currentAnswer; => right answer
    bool GO = 0;
    bool win = 0;
    
    
    Font answerFont;
    string rightAnswer; //what I draw
    string tempRightAnswer; //raw answer receive from sender
    int bodypart;
    
    inputArea   inputArea;
    wrongAnswer wrongAnswer;
    
    //--------all image about that Man
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

void HangManGuestApp::setup()
{
    // initialize net connection---------------------
    listener.setup(3000); // listener port
    host = "149.31.146.148"; // judge's IP
    sender.setup(host,4000,true);
    
    osc::Message    askID;
    askID.setAddress("/AskID/1");
    askID.addStringArg(System::getIpAddress());
    sender.sendMessage(askID);
    
    setWindowSize(800, 600);
    answerFont = Font(loadAsset( "Comic Sans MS Bold.ttf"), 60);
    
    bodypart = 0;
    bActivated = false;
    player = 0;
    
    inputArea.setup();
    wrongAnswer.setup();

}

void HangManGuestApp::update()
{
    while (listener.hasWaitingMessages()) {
        osc::Message    message;
        listener.getNextMessage(&message);

        if (player == 0) {
            player = message.getArgAsInt32(0);
            cout<< "initialize player ID: " << player<< endl;
        }
        else
        {
            int tempId = message.getArgAsInt32(0);
            
            if (tempId == player) {
                cout<< "player ID: " << player<< endl;
                
                
                answerLength = message.getArgAsInt32(1);
                cout<<"answerLength is : " << answerLength <<endl;
                
                rightAnswer = message.getArgAsString(2);
                cout<<"rightAnswer is : " << rightAnswer <<endl;
                
                wrongAnswer.setWrongAnswers(message.getArgAsString(3));
                cout<<"wrongAnswer is : " << wrongAnswer.getWrongAnswers() <<endl;
                
                bodypart = message.getArgAsInt32(4);
                cout<<"bodypart is : " << bodypart <<endl;
                
                GO = message.getArgAsInt32(5);
                cout<<"rightAnswer is : " << rightAnswer <<endl;
                
                bActivated = true;
            }
        }
    }
    if (bActivated) {
        inputArea.enableTextField();
    }
}

string HangManGuestApp::modifyAnswer(string answer)
{
    string newAnswer;
    for (int i = 0; i < answer.size(); i++) {
        newAnswer = newAnswer + answer[i] + " ";
    }
    //cout<<newAnswer<<endl;
    return newAnswer;
}


void HangManGuestApp::drawAnswer()
{
    gl::drawString( rightAnswer, ci::vec2( 30.f, 200.f ),Color::black(),answerFont);
}


void HangManGuestApp::drawMan()
{
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
    }

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
    drawAnswer();
    inputArea.draw();
    wrongAnswer.draw();
    rightAnswer = modifyAnswer(tempRightAnswer);
    
}

void HangManGuestApp::mouseDown(MouseEvent event)
{
//    bActivated = true;
}

void HangManGuestApp::keyDown(KeyEvent event)
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
                osc::Message    mMessage;
                mMessage.setAddress("/inputText");
                mMessage.addStringArg(inputArea.getInputText());
                sender.sendMessage(mMessage);
                
                bActivated = false;
                inputArea.reset();
                inputArea.disableTextField();
                
            }
        }
    }
}


CINDER_APP( HangManGuestApp, RendererGl )