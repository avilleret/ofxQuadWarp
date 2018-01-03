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

    void setNormalize(bool b){
        bNormalize=b;
    }

    ofMatrix4x4 getMatrix() const;
    ofMatrix4x4 getMatrixScaled(ofVec3f src_scale, ofVec3f dst_scale) const;
    ofMatrix4x4 getMatrixInverse() const;
    ofMatrix4x4 getMatrix(const std::vector<ofPoint>& srcPoints, const std::vector<ofPoint>& dstPoints) const;

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
    void drawQuadOutline();
    void drawQuadOutline(const std::vector<ofPoint>& point);
    void drawCorners();
    void drawCorners(const std::vector<ofPoint>& points);
    void drawHighlightedCorner(const std::vector<ofPoint>& points);
    void drawHighlightedCorner();
    void drawSelectedCorner();
    void drawSelectedCorner(const std::vector<ofPoint>& points);
    void drawCornerAt(const ofPoint& point);

    void onMouseMoved(ofMouseEventArgs & mouseArgs);
    void onMousePressed(ofMouseEventArgs & mouseArgs);
    void onMouseDragged(ofMouseEventArgs & mouseArgs);
    void onMouseReleased(ofMouseEventArgs & mouseArgs);
    void keyPressed(ofKeyEventArgs & keyArgs);
    void keyReleased(ofKeyEventArgs & keyArgs);

    bool bShow{}, bShowSrc{};

protected:
    std::vector<ofPoint> srcPoints;
    std::vector<ofPoint> dstPoints;

    float anchorSize{};
    int selectedCornerIndex{};
    int highlightCornerIndex{};

    bool bMouseEnabled{}, bMouseEnabledSrc{};
    bool bKeyboardShortcuts{}, bKeyboardShortcutsSrc{};

    bool bShiftPressed{};

    // wether the mouse coordinate are normalized (or not) against ofWindow size
    bool bNormalize = false;
};
