#pragma once
#include "ofMain.h"
#include "ofxALSParser.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
private:
    ofxALS::LiveSet live_set;

};
