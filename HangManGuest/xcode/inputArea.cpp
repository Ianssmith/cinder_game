//
//  inputArea.cpp
//  HangMan
//
//  Created by Neil on 3/14/17.
//
//

#include "inputArea.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

inputArea::inputArea(){};

void inputArea::setup()
{
    mFont = Font(loadAsset("Comic Sans MS Bold.ttf"), 18);
    mText = "";
    mTextPos = ci::vec2(215, 440);
    mTextFieldPos = ci::vec2(82, 400);
    mTextFieldSize = ci::vec2(300, 90);
    mLable = "Your Answer";
    mLablePos = ci::vec2(82, 350);
    editable = false;
    
}

void inputArea::enableTextField()
{
    editable = true;
    mLable = "Your Answer";
}

void inputArea::disableTextField()
{
    editable = false;
    mLable = "Waiting others";
}

void inputArea::reset()
{
    mText = "";
    mTextPos = ci::vec2(215, 440);
    mTextFieldPos = ci::vec2(82, 400);
    mTextFieldSize = ci::vec2(300, 90);
    mLable = "Your Answer";
    editable = true;
}

void inputArea::draw()
{
    gl::color(0.917, 0.717, 0.109);
    
    if(editable)
    {
        gl::drawSolidRect(Rectf(mTextFieldPos,mTextFieldPos+mTextFieldSize));
        gl::drawString(mText, mTextPos,ColorA(1,1,1), mFont);
    }
    
    gl::drawString(mLable, mLablePos, ColorA(0,0,0),mFont);
    
}

void  inputArea::setText(std::string inputText)
{
    mText = inputText;
}





