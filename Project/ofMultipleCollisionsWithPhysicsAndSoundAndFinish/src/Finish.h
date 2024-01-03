#pragma once
#include "ofMain.h"
#include "ofPath.h"

class drawFinish
{
public:
    drawFinish() {}
    
    // some members of drawFinish class are constants and some are set in the constructor.
    // Which members need to be set in this constructor? And which parameters can be removed and made into constants?
    drawFinish(const glm::vec2& pPos, const int pDegrees, const int pRotation) : pos(pPos), degrees(pDegrees), rotation(pRotation)  {
        checkAndAdjustPositions();
        for(int i = 0; i <= degrees; i++){
            float rad = ofDegToRad(i + rotation);
            float x = pos.x + radius * cos(rad);
            float y = pos.y + radius * sin(rad);
            line.addVertex(ofPoint(x, y));
        }
    }
    
    void draw()
    {
        ofSetColor(color);
        ofSetLineWidth(5);
        line.draw();
    }
    
    glm::vec2 getPos() {
        return pos;
    }
    
    float getRadius() {
        return radius;
    }
    
    ofPolyline line;

private:
    // parameters
    glm::vec2 pos;
    int degrees;
    int rotation;
    
    //constants
    float radius = ofGetWindowWidth() / 20;
    ofColor color = ofColor(255, 0, 0);
    
    // checks that center of circle is at least 1,5 x radius away from all edges
    void checkAndAdjustPositions() {
        float minMarginToEdge = radius * 1.5;
        if (pos.x < 0 + minMarginToEdge){
            pos.x = minMarginToEdge;
        }else if (pos.x > ofGetWindowWidth() - minMarginToEdge){
            pos.x = ofGetWindowWidth() - minMarginToEdge;
        }
        if (pos.y < 0 + minMarginToEdge){
            pos.y = minMarginToEdge;
        }else if (pos.y > ofGetWindowHeight() - minMarginToEdge){
            pos.y = ofGetWindowHeight() - minMarginToEdge;
        }
    }
};
