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
    
    const uint16_t newPort  = 4000;
    
    //void onSendError( asio::error_code error );
    bool mIsConnected;
    
    //// game functions v
    
    void setAnswer();
    void activatePlayer();
    void recieveChar();
    void compareChar(string);
    void compareAnswers();
    void checkDead();
    void makeMessage();
    
    //// game vars v
    
<<<<<<< HEAD
    ci::TextBox txtbox = TextBox().alignment( TextBox::CENTER ).text( "Enter word: ");
=======
    /// message vars
    int player = 0;     //player turn tracker
    int answerLength;
    string currentAnswer;
    string wrongAnswer;
    int bodypart = 0;
    bool GO = 0;        //boolean flag for if game is over
    bool win = 0;       //boolean flag for players win or lose
    
    /// host vars
    ci::TextBox txtbox;
>>>>>>> de9e86e0c665011444ff1f98737f64b0b43338b2
    string answer;      //answer chosen by judger
    const int maxGuesses = 9;
    char letter;        //letter guessed by player n
    std::vector<char> playerstatus;     //vector of boolean values to keep track of correct letters and if game is over
    string rLetter;       //char to return to players if guess is incorrect(probably could just recycle the 'letter' variable
    bool correct;       //boolean flag for if the guess is correct
    bool gameStart = 1;
    const std::string intro = "Judgment day, pick a word for the defendant: ";      //introductory text for judger
    int numPlayers = 0;
<<<<<<< HEAD
    gl::TextureRef		tex;
=======
    osc::Message message;
    
    
>>>>>>> de9e86e0c665011444ff1f98737f64b0b43338b2
    
    ////// Listener vvvv
    osc::Listener listener;
    
    
    
    
    
    
    
    ////// Listener class ^^^^
};
void oscGameApp::setup()
{
    //listener setup vvv
    listener.setup(4000);
    
    setAnswer();
    
    //sender setup vvv
    // assume the broadcast address is this machine's IP address but with 255 as the final value
    // so to multicast from IP 192.168.1.100, the host should be 192.168.1.255
    
    //host = System::getIpAddress();
    host = "149.31.138.40";
    //    if( host.rfind( '.' ) != string::npos )
    //        host.replace( host.rfind( '.' ) + 1, 3, "255" );
    sender.setup( host, port, 1 );
}

void oscGameApp::update()
{
    //// Listen for incoming messages
    recieveChar();
}



void oscGameApp::draw()
{
    gl::clear();
    gl::drawString(intro, vec2(getWindowWidth()*0.25f,getWindowHeight()*0.25f),Color(1.0,0.0,0.0));
    //gl::color( Color::gray( 0.5f ) );
    //gl::drawSolidRect(Rectf(vec2(0), vec2(positionX * getWindowWidth(), getWindowHeight())));
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
    
    txtbox.setAlignment(cinder::TextBox::CENTER);
    txtbox.setSize(vec2(250,42));
    txtbox.setColor(Color(0.0f,0.1f,0.6f));
    //txtbox.setBackgroundColor(Color(0.0,0.1,0.6));
    //txtbox.text(
    tex = gl::Texture2d::create(txtbox.render());
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
        setAnswer();
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
    if(tex)
    {
        gl::draw(tex);
    }
    //gl::color( Color::gray( 0.5f ) );
    //gl::drawSolidRect(Rectf(vec2(0), vec2(positionX * getWindowWidth(), getWindowHeight())));
}



//------
//
// game functions definitions
//
//------

////create a vector equal to the length of the answer and store a 'false' in each space
void oscGameApp::setAnswer()
{
<<<<<<< HEAD
    //answer = "test";
=======
    answer = "test";
    txtbox.setAlignment(cinder::TextBox::CENTER);
    txtbox.setSize(vec2(250,42));
    txtbox.setText("Enter word: ");
    txtbox.setColor(Color(0.0f,0.1f,0.6f));
    txtbox.setBackgroundColor(Color(0.0,0.1,0.6));
>>>>>>> de9e86e0c665011444ff1f98737f64b0b43338b2
    //answer = inputBox.getInput();     place where judger word is received
    answerLength = answer.size();
    for(int i=0;i<answer.size();i++)
    {
        playerstatus.push_back('_');
    }
    while(numPlayers < 2)
    {
        numPlayers++;
        //app.onPlayerJoin(sendmsg());    place where we wait for and detect new players
    }
    wrongAnswer = "";
    gameStart = 0;
}

////adds 1 to player number each time until it equals total players then reset to first player
void oscGameApp::activatePlayer()
{
    if (player > numPlayers)
    {
        player = 1;
    }
    player += 1;
    
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
        
        if(inmsg.getAddress() == "/hangman/1" &&
           inmsg.getNumArgs() == 2 &&
           inmsg.getArgType(0) == osc::TYPE_STRING &&
           inmsg.getArgType(1) == osc::TYPE_INT32)
        {
            rLetter = inmsg.getArgAsString(0);
            player = inmsg.getArgAsInt32(1);
            
            letter = rLetter[0];
        }else if(inmsg.getAddress() == "/AskID"){
            player += 1;
            message.setAddress("/SendID");
            message.addIntArg(player);
            message.setRemoteEndpoint(host, port);
            sender.sendMessage(message);
            
        }
        
        cout << "here is running " << endl;
        player = inmsg.getArgAsInt32(0);
        rLetter = inmsg.getArgAsString(1);
        cout << "player is : " << player <<endl;
        cout << "right letter is : " << rLetter <<endl;
    }
}


////loop through the answer and compare the guessed char to each one
////if they match change the corresponding vector boolean to 'true'
////and set correct flag
void oscGameApp::compareChar(string guess)
{
    correct = 0;
    for(std::string::size_type i=0;i<answer.size();i++)
    {
        if(answer[i] == guess[0])
        {
            playerstatus[i] = guess[0];
            correct = 1;
        }
    }
    if(correct == 0)
    {
        wrongAnswer = wrongAnswer + guess[0];
        //rLetter = letter;
    }
}


////check the status array for the presence of a false
////if all elements are true game over and players win
void oscGameApp::compareAnswers()
{
    if(std::all_of(playerstatus.begin(), playerstatus.end(), [](int i){return i=='_';}))
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
void oscGameApp::makeMessage()
{
    activatePlayer();
    message.addIntArg(player);
    message.addIntArg(answerLength);     //Use <- this variable to initialze the length of the boolean vector for the first time.
    compareChar(rLetter);
    currentAnswer = "";
    //msg.addStringArg(rLetter);
    //msg.addIntArg(correct);
    for(int i=0;i<playerstatus.size();i++)
    {
        currentAnswer = currentAnswer + playerstatus[i];
        //msg.addIntArg(playerstatus[i]);
    }
    message.addStringArg(currentAnswer);
    message.addStringArg(wrongAnswer);
    if(correct == 0)
    {
        bodypart += 1;
        message.addIntArg(bodypart);
    }
    compareAnswers();
    message.addIntArg(GO);
    if(GO == 1)
    {
        message.addIntArg(win);
    }
}

<<<<<<< HEAD
=======



void oscGameApp::keyUp(KeyEvent event)
{
    if(event.getCode() == KeyEvent::KEY_RETURN && gameStart == 0)
    {
        message.setAddress("/cinder/osc/1");
        //message.addFloatArg(positionX);
        makeMessage();
        //        cout<<m<<endl;
        message.setRemoteEndpoint(host, port);
        sender.sendMessage(message);
    }
    
    if(event.getCode() == KeyEvent::KEY_RETURN && gameStart == 1)
    {
        //setAnswer();
        //answer = txtbox.getText();
        //cout << answer << endl;
    }
    
}


void oscGameApp::mouseMove( MouseEvent event )
{
}

void oscGameApp::mouseDrag( MouseEvent event )
{
}

>>>>>>> de9e86e0c665011444ff1f98737f64b0b43338b2

CINDER_APP( oscGameApp, RendererGl )
