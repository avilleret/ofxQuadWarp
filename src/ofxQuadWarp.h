//
//  ofxQuadWarp.h
//  Created by lukasz karluk on 19/06/11.
//
//  nay added hack to invert on 2011/06/21
//
#pragma once

#include "ofMain.h"

class ofxQuadWarp 
{
public: 
    
     ofxQuadWarp();
    ~ofxQuadWarp();
    
    void setup();
    
    void setAnchorSize(float value);
    
    void setSourceRect(const ofRectangle& rect);
    void setTargetRect(const ofRectangle& rect);
    void setTargetPoints(const vector<ofPoint>& points);
    void setSourcePoints(const vector<ofPoint>& points);
    vector<ofPoint> getTargetPoints();
    vector<ofPoint> getSourcePoints();
    
    void enableMouseControls();
    void disableMouseControls();
    
    void enableKeyboardShortcuts();
    void disableKeyboardShortcuts();

    void enableMouseControlsSrc();
    void disableMouseControlsSrc();

    void enableKeyboardShortcutsSrc();
    void disableKeyboardShortcutsSrc();

    bool hitTest(ofVec2f pos); // checks whether `pos` is within the warped quad
    ofRectangle boundingBox(); // calculates and returns a bounding box
    
    void update();
    void reset();
    
    ofMatrix4x4 getMatrix() const;
    ofMatrix4x4 getMatrixInverse() const;
    ofMatrix4x4 getMatrix(const ofPoint * srcPoints, const ofPoint * dstPoints) const;
    
    void show();
    void hide();
    void toggleShow();
    void showSrc();
    void hideSrc();
    void toggleShowSrc();
    bool isShowing();
    
    void save(const string& path="quadwarp.xml");
    void load(const string& path="quadwarp.xml");
    
    void draw();
    void drawSrc();
    void drawQuadOutline();
    void drawQuadOutline(ofPoint* point);
    void drawCorners();
    void drawCorners(ofPoint* points);
    void drawHighlightedCorner(ofPoint* points);
    void drawHighlightedCorner();
    void drawSelectedCorner();
    void drawSelectedCorner(ofPoint* points);
    void drawCornerAt(const ofPoint& point);

    void onMouseMoved(ofMouseEventArgs & mouseArgs);
    void onMousePressed(ofMouseEventArgs & mouseArgs);
    void onMouseDragged(ofMouseEventArgs & mouseArgs);
    void onMouseReleased(ofMouseEventArgs & mouseArgs);
    void keyPressed(ofKeyEventArgs & keyArgs);
    void keyReleased(ofKeyEventArgs & keyArgs);

    void onMouseMovedSrc(ofMouseEventArgs & mouseArgs);
    void onMousePressedSrc(ofMouseEventArgs & mouseArgs);
    void onMouseDraggedSrc(ofMouseEventArgs & mouseArgs);
    void onMouseReleasedSrc(ofMouseEventArgs & mouseArgs);
    void keyPressedSrc(ofKeyEventArgs & keyArgs);

protected:
    ofPoint srcPoints[4];
    ofPoint dstPoints[4];
    
    float anchorSize;
    int selectedCornerIndex;
    int highlightCornerIndex;
    
    bool bMouseEnabled, bMouseEnabledSrc;
    bool bKeyboardShortcuts, bKeyboardShortcutsSrc;
    bool bShow, bShowSrc;

    bool bShiftPressed;
};
