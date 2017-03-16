//
//  wrongAnswers.cpp
//  HangMan
//
//  Created by Neil on 3/15/17.
//
//

#include "wrongAnswers.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

wrongAnswer::wrongAnswer(){}

void wrongAnswer::setWrongAnswers(std::string wrongAnswer)
{
    wrongAnswers = wrongAnswer;
}

void wrongAnswer::setup()
{
    mFont = Font(loadAsset("Comic Sans MS Bold.ttf"),20);
    
    wrongAnswerLablePos = ci::vec2(500, 460);
    wrongAnswerLable = "Wrong Answers: ";
    wrongAnswersPos = ci::vec2(500, 500);
    wrongAnswers = "";
}

void wrongAnswer::reset()
{
    wrongAnswers = "";
}

void wrongAnswer::draw()
{

    gl::drawString(wrongAnswerLable, wrongAnswerLablePos,ColorA(0,0,0),mFont);
    gl::drawString(wrongAnswers, wrongAnswersPos, ColorA(0,0,0),mFont);
    

}
