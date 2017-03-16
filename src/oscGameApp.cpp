#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"
#include "OscSender.h"
#include "OscListener.h"
#include "cinder/Text.h"

using namespace ci;
using namespace ci::app;
using namespace std;



class oscGameApp : public App {
public:
    
    int 		mMouseLocX;
    osc::Sender sender;
    //std::string host;
    
    void setup();
    void update();
    void draw();
    void mouseMove( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void keyUp( KeyEvent event );
    
    //osc::SenderTcp sender;
    float 		positionX;
    //const std::string host = "";
    std::string host;
    const uint16_t port = 3000;
    
    //void onSendError( asio::error_code error );
    bool mIsConnected;
    
    //// game functions v
    
    void setAnswer();
    void activatePlayer();
    void recieveChar();
    void compareChar(char);
    void compareAnswers();
    void checkDead();
    void makeMessage(osc::Message);
    
    //// game vars v
    
    ci::TextBox txtbox;
    string answer;      //answer chosen by judger
    int answerLength;
    int player = 0;     //player turn tracker
    const int maxGuesses = 9;
    int bodypart = 0;
    char letter;        //letter guessed by player n
    std::vector<bool> playerstatus;     //vector of boolean values to keep track of correct letters and if game is over
    string rLetter;       //char to return to players if guess is incorrect(probably could just recycle the 'letter' variable
    bool correct;       //boolean flag for if the guess is correct
    bool gameStart = 1;
    bool GO = 0;        //boolean flag for if game is over
    bool win = 0;       //boolean flag for players win or lose
    const std::string intro = "Judgment day, pick a word for the defendant: ";      //introductory text for judger
    int numPlayers = 0;
    
    
    ////// Listener vvvv
    osc::Listener listener;
    






    ////// Listener class ^^^^
};



////create a vector equal to the length of the answer and store a 'false' in each space
void oscGameApp::setAnswer()
{
    //answer = "test";
    txtbox.setAlignment(cinder::TextBox::CENTER);
    txtbox.setSize(vec2(250,42));
    txtbox.setText("Enter word: ");
    txtbox.setColor(Color(0.0f,0.1f,0.6f));
    txtbox.setBackgroundColor(Color(0.0,0.1,0.6));
    //answer = inputBox.getInput();     place where judger word is received
    answerLength = answer.size();
    for(int i=0;i<answer.size();i++)
    {
        playerstatus.push_back(0);
    }
    while(numPlayers < 2)
    {
        numPlayers++;
        //app.onPlayerJoin(sendmsg());    place where we wait for and detect new players
    }
    gameStart = 0;
}

////adds 1 to player number each time until it equals total players then reset to first player
void oscGameApp::activatePlayer()
{
    player += 1;
    if (player > numPlayers)
    {
        player = 1;
    }
}


////place where we get the char guessed by players
void oscGameApp::recieveChar()
{
    //letter = 't';
    //letter = app.getMessage[0];
    while(listener.hasWaitingMessages())
    {
        osc::Message inmsg;
        listener.getNextMessage(&inmsg);
        
        if(inmsg.getAddress() == "/cinder/osc/1" &&
           inmsg.getNumArgs() == 2 &&
           inmsg.getArgType(0) == osc::TYPE_STRING &&
           inmsg.getArgType(1) == osc::TYPE_INT32)
        {
            rLetter = inmsg.getArgAsString(0);
            player = inmsg.getArgAsInt32(1);
            
            letter = rLetter[0];
        }
    }
}


////loop through the answer and compare the guessed char to each one
////if they match change the corresponding vector boolean to 'true'
////and set correct flag
void oscGameApp::compareChar(char guess)
{
    correct = 0;
    for(std::string::size_type i=0;i<answer.size();i++)
    {
        if(answer[i] == guess)
        {
            playerstatus[i] = 1;
            correct = 1;
        }
    }
    //rLetter = letter;
}


////check the status array for the presence of a false
////if all elements are true game over and players win
void oscGameApp::compareAnswers()
{
    if(std::all_of(playerstatus.begin(), playerstatus.end(), [](int i){return i==0;}))
    {
        GO = 0;
    }
    else
    {
        GO = 1;
        win = 1;
    }
}


////if players have all body parts gome over and players lose
   void oscGameApp::checkDead()
   {
       if(bodypart == maxGuesses)
       {
           GO = 1;
           compareAnswers();
       }
   }


////run through the above functions appending the generated values to a message for players
    void oscGameApp::makeMessage(osc::Message msg)
    {
       activatePlayer();
       msg.addIntArg(answerLength);     //Use <- this variable to initialze the length of the boolean vector for the first time.
       msg.addIntArg(player);
       compareChar(letter);
       msg.addStringArg(rLetter);
       msg.addIntArg(correct);
       for(int i=0;i<playerstatus.size();i++)
       {
           msg.addIntArg(playerstatus[i]);
       }
       if(correct == 0)
       {
           bodypart += 1;
           msg.addIntArg(bodypart);
       }
       compareAnswers();
       msg.addIntArg(GO);
       if(GO == 1)
       {
           msg.addIntArg(win);
       }
   }
       


void oscGameApp::setup()
{
    //listener setup vvv
    listener.setup(3000);
    
    
    //sender setup vvv
    // assume the broadcast address is this machine's IP address but with 255 as the final value
    // so to multicast from IP 192.168.1.100, the host should be 192.168.1.255

    //host = System::getIpAddress();
    host = "192.168.1.139";
    if( host.rfind( '.' ) != string::npos )
        host.replace( host.rfind( '.' ) + 1, 3, "255" );
    sender.setup( host, port, 1 );
    setAnswer();
}

void oscGameApp::update()
{
    //// Listen for incoming messages
    recieveChar();
}

void oscGameApp::keyUp(KeyEvent event)
{
    if(event.getCode() == KeyEvent::KEY_RETURN && gameStart == 0)
    {
        osc::Message message;
        message.setAddress("/cinder/osc/1");
        //message.addFloatArg(positionX);
        makeMessage(message);
        cout<<&message<<endl;
        message.setRemoteEndpoint(host, port);
        sender.sendMessage(message);
    }
    
    if(event.getCode() == KeyEvent::KEY_RETURN && gameStart == 1)
    {
        answer = txtbox.getText();
        cout << answer << endl;
    }
    
}


void oscGameApp::mouseMove( MouseEvent event )
{
}

void oscGameApp::mouseDrag( MouseEvent event )
{
}

void oscGameApp::draw()
{
    gl::clear();
    gl::drawString(intro, vec2(getWindowWidth()*0.25f,getWindowHeight()*0.25f),Color(1.0,0.0,0.0));
    setAnswer();
    //gl::color( Color::gray( 0.5f ) );
    //gl::drawSolidRect(Rectf(vec2(0), vec2(positionX * getWindowWidth(), getWindowHeight())));
}

CINDER_APP( oscGameApp, RendererGl )
