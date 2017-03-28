//
//  wrongAnswers.hpp
//  HangMan
//
//  Created by Neil on 3/15/17.
//
//

#ifndef wrongAnswers_hpp
#define wrongAnswers_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/DataSource.h"
#include "cinder/Font.h"

#include "cinder/System.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class wrongAnswer
{
public:
    wrongAnswer();
    void setWrongAnswers(std::string wrongAnswer);
    string getWrongAnswers(){return wrongAnswers;}
    void setup();
    void reset();
    void draw();
    
    
private:
    std::string wrongAnswers;
    ci::vec2    wrongAnswersPos;
    ci::vec2    wrongAnswerLablePos;
    std::string wrongAnswerLable;
    
    Font        mFont;
};

#endif /* wrongAnswers_hpp */
