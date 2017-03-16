//
//  inputArea.hpp
//  HangMan
//
//  Created by Neil on 3/14/17.
//
//

#ifndef inputArea_hpp
#define inputArea_hpp

#include <stdio.h>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Font.h"
#include "cinder/DataSource.h"

#include "cinder/System.h"

class inputArea{
    
public:
    inputArea();
    void setup();// mText = null; set up all position and size
    void enableTextField();//mLable = "Your answer", show input line
    void disableTextField();//mLable = "wait for other"; input line disappear;
    void reset();
    void draw();// draw input line, draw mLable
    std::string getInputText(){return mText;}
    
    bool editable;
    void setText(std::string inputText);
    

    
private:
    std::string     mText;
    ci::vec2        mTextPos;
    ci::vec2        mTextFieldPos;
    ci::vec2        mTextFieldSize;
    std::string     mLable;
    ci::vec2        mLablePos;
    
    ci::Font            mFont;
    
    };

#endif /* inputArea_hpp */
