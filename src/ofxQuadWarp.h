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
    vector<glm::vec3>& getTargetPoints();
    vector<glm::vec3>& getSourcePoints();

    void enableMouseControls();
    void disableMouseControls();

    void enableKeyboardShortcuts();
    void disableKeyboardShortcuts();

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
    ofMatrix4x4 getMatrix(const std::vector<glm::vec3>& srcPoints, const std::vector<glm::vec3>& dstPoints) const;

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
    void drawQuadOutline(const std::vector<glm::vec3>& point);
    void drawCorners();
    void drawCorners(const std::vector<glm::vec3>& points);
    void drawHighlightedCorner(const std::vector<glm::vec3>& points);
    void drawHighlightedCorner();
    void drawSelectedCorner();
    void drawSelectedCorner(const std::vector<glm::vec3>& points);
    void drawCornerAt(const ofPoint& point);

    void onMouseMoved(ofMouseEventArgs & mouseArgs);
    void onMousePressed(ofMouseEventArgs & mouseArgs);
    void onMouseDragged(ofMouseEventArgs & mouseArgs);
    void onMouseReleased(ofMouseEventArgs & mouseArgs);
    void keyPressed(ofKeyEventArgs & keyArgs);
    void keyReleased(ofKeyEventArgs & keyArgs);

    bool bShow{}, bShowSrc{};

protected:
    std::vector<glm::vec3> srcPoints;
    std::vector<glm::vec3> dstPoints;

    float anchorSize{};
    int selectedCornerIndex{};
    int highlightCornerIndex{};

    bool bMouseEnabled{}, bMouseEnabledSrc{};
    bool bKeyboardShortcuts{}, bKeyboardShortcutsSrc{};

    bool bShiftPressed{};

    // wether the mouse coordinate are normalized (or not) against ofWindow size
    bool bNormalize = false;
};
