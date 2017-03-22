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
    void gameOver();
    
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
    
    int counter = 0;
    osc::Message    mMessage;
    
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
    host = "192.168.1.255"; // judge's IP
    sender.setup(host,4000,true);
    
    osc::Message    askID;
    askID.setAddress("/cinder/osc");
    askID.addStringArg(System::getIpAddress());
    sender.sendMessage(askID);
    
    setWindowSize(800, 600);
    answerFont = Font(loadAsset( "Comic Sans MS Bold.ttf"), 40);
    
    bodypart = 0;
    bActivated = false;
    player = 0;
    answerLength = 0;
    
    inputArea.setup();
    wrongAnswer.setup();

}

void HangManGuestApp::update()
{
    while (listener.hasWaitingMessages()) {
        counter++;
        cout << "counter is: " << counter <<endl;
        osc::Message    message;
        listener.getNextMessage(&message);

        if (player == 0) {
            cout << "Arguments number: " << message.getNumArgs() <<endl;
            player = message.getArgAsInt32(0);
            cout<< "initialize player ID: " << player<< endl;
        }
        
        else if(player != 0)
        {
            cout << "Arguments number: " << message.getNumArgs() <<endl;

            int tempId = message.getArgAsInt32(0);
            cout << "recieve second msg" <<endl;
            
            for (int i = 0; i < message.getNumArgs(); i++) {
                if(message.getArgType(i) == osc::TYPE_INT32)
                    cout << "arg["<<i<<"] is : "<< message.getArgAsInt32(i) <<endl;
                if(message.getArgType(i) == osc::TYPE_STRING)
                    cout << "arg["<<i<<"] is : "<< message.getArgAsString(i)<<endl;

            }
            
            if (tempId == player) {
                cout<< "player ID: " << player<< endl;
                
                answerLength = message.getArgAsInt32(1);
                cout<<"answerLength is : " << answerLength <<endl;
                
                tempRightAnswer = message.getArgAsString(2);
                cout<<"rightAnswer is : " << tempRightAnswer <<endl;
                
                wrongAnswer.setWrongAnswers(message.getArgAsString(3));
                cout<<"wrongAnswer is : " << wrongAnswer.getWrongAnswers() <<endl;
                
                bodypart = message.getArgAsInt32(4);
                cout<<"bodypart is : " << bodypart <<endl;
                
                GO = message.getArgAsInt32(5);
                cout<<"GO is : " << GO <<endl;
                
                bActivated = true;
            }else
                
            {
                //can still receive the data when I am not the current player
                answerLength = message.getArgAsInt32(1);
                cout<<"answerLength is : " << answerLength <<endl;
                
                tempRightAnswer = message.getArgAsString(2);
                cout<<"rightAnswer is : " << tempRightAnswer <<endl;
                
                wrongAnswer.setWrongAnswers(message.getArgAsString(3));
                cout<<"wrongAnswer is : " << wrongAnswer.getWrongAnswers() <<endl;
                
                bodypart = message.getArgAsInt32(4);
                cout<<"bodypart is : " << bodypart <<endl;
                
                GO = message.getArgAsInt32(5);
                cout<<"GO is : " << GO <<endl;
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
        if (bodypart < 9){
            gl::draw (mBodyPart[i],Rectf(525, 110, 675, 400));
        }else if (bodypart == 9) {
            gl::draw (mBodyPart[0],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[1],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[2],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[3],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[4],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[5],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[6],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[7],Rectf(525, 110, 675, 400));
            gl::draw (mBodyPart[8],Rectf(525, 110, 675, 400));
            gameOver();
            inputArea.disableTextField();
        }
    }
}

void HangManGuestApp::gameOver()
{
    gl::color(ci::Color(1.f, 0, 0));
    gl::drawSolidRect(Rectf(170, 230, 620, 380));
    gl::drawString("G A M E  O V E R", ci::vec2(220.f, 280.f),Color::white(), answerFont);
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
    
    inputArea.draw();
    wrongAnswer.draw();
    
    rightAnswer = modifyAnswer(tempRightAnswer);
    drawAnswer();
    
    //start drawing when there is wrong answer
//    if (wrongAnswer.getWrongAnswers() != "") {
//        drawMan();
//    }
    drawMan();
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
                mMessage.clear();
                mMessage.setAddress("/cinder/osc");
                mMessage.addStringArg(inputArea.getInputText());
                mMessage.addIntArg(player);
                sender.sendMessage(mMessage);
                
                for (int i = 0; i < mMessage.getNumArgs(); i++) {
                    if(mMessage.getArgType(i) == osc::TYPE_INT32)
                        cout << "arg["<<i<<"] is : "<< mMessage.getArgAsInt32(i) <<endl;
                    if(mMessage.getArgType(i) == osc::TYPE_STRING)
                        cout << "arg["<<i<<"] is : "<< mMessage.getArgAsString(i)<<endl;
                }
                
                bActivated = false;
                inputArea.reset();
                inputArea.disableTextField();

            }
        }
    }
}


CINDER_APP( HangManGuestApp, RendererGl )
