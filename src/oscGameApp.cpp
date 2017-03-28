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



class testGameApp : public App {
public:
    
    void setup();
    void update();
    void draw();
    void mouseMove( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void keyUp( KeyEvent event );
    
    osc::Sender sender;
    std::string host;
    const uint16_t sendPort = 3000;
    
    void setAnswer();
    void activatePlayer();
    void compareChar(string);
    void compareAnswers();
    void checkDead();
    string makeMessage(string);
    
    /// message vars
    int playerID;     //player turn tracker
    int numPlayers;
    string answer;      //answer chosen by judger
    int answerLength;
    //string currentAnswer;
    string wrongAnswer;
    
    int bodypart;
    bool gameStart;
    bool GO;        //boolean flag for if game is over
    bool win;       //boolean flag for players win or lose
    
    /// host vars
    ci::TextBox txtbox;
    int maxGuesses;
    char letter;        //letter guessed by player n
    std::vector<char> currentAnswer;     //vector of boolean values to keep track of correct letters and if game is over
    //string currentAnswerInString;
    string rLetter;       //char to return to players if guess is incorrect(probably could just recycle the 'letter' variable
    bool correct;       //boolean flag for if the guess is correct
    const std::string intro = "Judgment day, pick a word for the defendant: ";      //introductory text for judger
    
    osc::Message message;
    
    osc::Listener listener;
    const uint16_t recievePort  = 4000;
    
    string boxtext;
    gl::TextureRef tex;
    ci::Rectf rect;
    bool wordbox = true;
    bool answerIsInput = false;
    
    string showProgress = "";
    
};
void testGameApp::setup()
{
    
    listener.setup(4000);
    playerID = 0;
    numPlayers = 0;
    bodypart = 0;
    GO = 0;
    win = 0;
    maxGuesses = 9;
    gameStart = 0;
    
    
    answer = wrongAnswer = "";
    //setAnswer(); //initialize the answerLength and answer, currentAnswer[];
    
    //sender setup vvv
    // assume the broadcast address is this machine's IP address but with 255 as the final value
    // so to multicast from IP 192.168.1.100, the host should be 192.168.1.255
    
    host = "149.31.207.32";
    //    if( host.rfind( '.' ) != string::npos )
    //        host.replace( host.rfind( '.' ) + 1, 3, "255" );
    sender.setup( host, sendPort, 1 );
    cout<<"all var:--------------- "<< endl;
    cout<<"answer: " << answer <<endl;
    cout<<"currentAnswer: " <<endl;
    cout << "wrong answers: " << wrongAnswer<<endl;
    
    
}

void testGameApp::setAnswer()
{
    //answer = "test";
    
    //answer = inputBox.getInput();     //place where judger word is received
    //answer = "apple";   //just for test
    while(!answerIsInput)
    {
        cout << "waiting for word" << endl;
    }
    
    answerLength = answer.size();
    
    for(int i=0;i<answer.size();i++)
    {
        currentAnswer.push_back('_');
    }
    //    while(numPlayers < 2)
    //    {
    //        numPlayers++;
    //        //app.onPlayerJoin(sendmsg());    place where we wait for and detect new players
    //    }
    //wrongAnswer = "";
    //gameStart = 0;
    answerIsInput = false;
}



void testGameApp::update()
{
    
    while(listener.hasWaitingMessages())
    {
        osc::Message inmsg;
        listener.getNextMessage(&inmsg);
        
        if (!gameStart) {
            string tempIP = inmsg.getArgAsString(0);
            numPlayers++;
            cout << numPlayers << endl;
            playerID ++;
            message.setAddress("/cinder/osc");
            message.addIntArg(playerID);
            message.setRemoteEndpoint(tempIP, sendPort);
            sender.sendMessage(message);
            cout << "send player ID: " << playerID <<endl;
            cout << "IP address: " << tempIP <<endl;
            cout << "-------------------------" <<endl;
            
            if (numPlayers >= 1) {
                gameStart = true;
                cout << "trigger message!" <<endl;
                showProgress = makeMessage(showProgress);
            }
            cout << "1 player join in. Now players number is: " << numPlayers <<endl;
        }
        
        else if( gameStart )
        {
            rLetter = inmsg.getArgAsString(0); // set right
            playerID = inmsg.getArgAsInt32(1);
            compareChar(rLetter);
            cout<<"normal message." <<endl;
            showProgress = makeMessage(showProgress);
        }
    }
    
}



void testGameApp::draw()
{
    if(!wordbox && !answerIsInput)
    {
        gl::clear();
        gl::drawString("player guessed: " + rLetter,vec2(getWindowWidth()*0.25f,getWindowHeight()*0.50f),Color(1.0,0.0,0.0));
        gl::drawString("player progress: " + showProgress,vec2(getWindowWidth()*0.75f,getWindowHeight()*0.75f),Color(1.0,0.0,0.0));
        gl::drawString("waiting for player guess", vec2(getWindowWidth()*0.25f,getWindowHeight()*0.25f),Color(1.0,0.0,0.0));
        
    }
    if(wordbox)
    {
        
        gl::clear();
        gl::drawString(intro, vec2(getWindowWidth()*0.25f,getWindowHeight()*0.25f),Color(1.0,0.0,0.0));
        //cout << "waiting for judger to choose word" << endl;
        txtbox.setAlignment(cinder::TextBox::CENTER);
        txtbox.setSize(vec2(250,42));
        txtbox.setText(answer);
        txtbox.setColor(Color(0.0f,0.0f,0.0f));
        txtbox.setBackgroundColor(Color(0.5,0.5,0.5));
        tex= gl::Texture2d::create(txtbox.render());
        
        gl::draw(tex);
    }
    if(!wordbox && answerIsInput)
    {
        setAnswer(); //initialize the answerLength and answer, currentAnswer[];
        cout << answer << endl;
        //cout << "Answer set" << endl;
    }
}



////create a vector equal to the length of the answer and store a 'false' in each space
////adds 1 to player number each time until it equals total players then reset to first player
void testGameApp::activatePlayer()
{
    if (playerID > numPlayers)
    {
        playerID = 1;
    }
    playerID += 1;
    
}

////loop through the answer and compare the guessed char to each one
////if they match change the corresponding vector boolean to 'true'
////and set correct flag
void testGameApp::compareChar(string guess)
{
    correct = 0;
    for(std::string::size_type i=0;i<answer.size();i++)
    {
        if(answer[i] == guess[0])
        {
            currentAnswer[i] = guess[0];
            correct = 1;
        }
    }
    if(correct == 0)
    {
        wrongAnswer = wrongAnswer + guess[0];
        checkDead();
        //rLetter = letter;
    }
}


void testGameApp::compareAnswers()
{
    if(std::all_of(currentAnswer.begin(), currentAnswer.end(), [](int i){return i=='_';}))
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
void testGameApp::checkDead()
{
    if(bodypart == maxGuesses)
    {
        GO = 1;
        compareAnswers();
    }
}


////run through the above functions appending the generated values to a message for players
string testGameApp::makeMessage(string progressForHostRender)
{
    message.clear();
    message.setAddress("/cinder/osc");
    message.addIntArg(playerID);
    message.addIntArg(answerLength);     //Use <- this variable to initialze the length of the boolean vector for the first time.
    
    string currentAnswerInString = "";
    for(int i=0;i<currentAnswer.size();i++)
    {
        currentAnswerInString += currentAnswer[i];
    }
    cout << "string sent will be: " << currentAnswerInString << endl;
    progressForHostRender = currentAnswerInString;
    message.addStringArg(currentAnswerInString);
    message.addStringArg(wrongAnswer);
    if(correct == 0)
    {
        bodypart += 1;
        message.addIntArg(bodypart);
    }else
    {
        message.addIntArg(bodypart);
    }
    compareAnswers();
    message.addIntArg(GO);
    
    if(GO == 1)
    {
        message.addIntArg(win);
    }
    
    sender.sendMessage(message);
    
    for (int i = 0 ; i < message.getNumArgs(); i++) {
        if (message.getArgType(i) == osc::TYPE_INT32) {
            cout << "arg[" << i << "] is : " << message.getArgAsInt32(i)<<endl;
        }
        if (message.getArgType(i) == osc::TYPE_STRING) {
            cout << "arg[" << i << "] is : " << message.getArgAsString(i) <<endl  ;
        }
    }
    
    //    cout << "Message is: " <<endl;
    //    cout << "player ID: " << playerID << endl;
    //    cout << "answerLength : " << answerLength <<endl;
    //    cout << "current Answer String: " << currentAnswerInString <<endl;
    //    cout << "wrong Answer: " << wrongAnswer <<endl;
    //    cout << "bodyPart: " << bodypart<<endl;
    //    cout << "game over? " << GO <<endl;
    cout << "--------------------------------------"<<endl;
    return progressForHostRender;
    
}




void testGameApp::keyUp(KeyEvent event)
{
    if(gameStart == 0)
    {
        cout << "keypress" << endl;
        if( event.getCode() == KeyEvent::KEY_BACKSPACE ){
            if( answer.size() > 0 ){
                answer = answer.substr( 0, answer.size()-1 );
            }
        } else if (event.getCode() != KeyEvent::KEY_RETURN) {
            const char character = event.getChar();
            answer += string( &character, 1 );
            cout << "answer size = " << answer.size() <<endl;
        }
    }
    if(event.getCode() == KeyEvent::KEY_RETURN && gameStart == 1)
    {
        message.setAddress("/cinder/osc");
        showProgress = makeMessage(showProgress);
        sender.sendMessage(message);
    }
    
    if(event.getCode() == KeyEvent::KEY_RETURN && gameStart == 0)
    {
        //setAnswer();
        //answer = txtbox.getText();
        //cout << answer << endl;
        wordbox = false;
        answerIsInput = true;
    }
    
}


void testGameApp::mouseMove( MouseEvent event )
{
}

void testGameApp::mouseDrag( MouseEvent event )
{
}


CINDER_APP( testGameApp, RendererGl )
