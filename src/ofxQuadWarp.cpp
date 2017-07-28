//
//  ofxQuadWarp.cpp
//  Created by lukasz karluk on 19/06/11.
//

#include "ofxQuadWarp.h"
#include "ofxHomography.h"

ofxQuadWarp::ofxQuadWarp() {
    anchorSize = 10;
    selectedCornerIndex = -1;
    highlightCornerIndex = -1;
    
    bMouseEnabled = false;
    bKeyboardShortcuts = false;
    bShow = false;
    bShiftPressed = false;
}

ofxQuadWarp::~ofxQuadWarp() {
    disableMouseControls();
    disableKeyboardShortcuts();
}

//----------------------------------------------------- setup.
void ofxQuadWarp::setup() {
    enableMouseControls();
    enableKeyboardShortcuts();
    show();
}

//----------------------------------------------------- setters.

void ofxQuadWarp::setAnchorSize(float value) {
    anchorSize = value;
}

//----------------------------------------------------- enable / disable.
void ofxQuadWarp::enableMouseControls() {
    if(bMouseEnabled == true) {
        return;
    }
    bMouseEnabled = true;
    ofAddListener(ofEvents().mouseMoved, this, &ofxQuadWarp::onMouseMoved);
    ofAddListener(ofEvents().mousePressed, this, &ofxQuadWarp::onMousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxQuadWarp::onMouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxQuadWarp::onMouseReleased);
}

void ofxQuadWarp::disableMouseControls() {
    if(bMouseEnabled == false) {
        return;
    }
    bMouseEnabled = false;
    try {
        ofRemoveListener(ofEvents().mouseMoved, this, &ofxQuadWarp::onMouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxQuadWarp::onMousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxQuadWarp::onMouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxQuadWarp::onMouseReleased);
    }
    catch(Poco::SystemException) {
        return;
    }
}

void ofxQuadWarp::enableKeyboardShortcuts() {
    if(bKeyboardShortcuts == true) {
        return;
    }
    bKeyboardShortcuts = true;
    ofAddListener(ofEvents().keyPressed, this, &ofxQuadWarp::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxQuadWarp::keyReleased);
}

void ofxQuadWarp::disableKeyboardShortcuts() {
    if(bKeyboardShortcuts == false) {
        return;
    }
    bKeyboardShortcuts = false;
    try {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxQuadWarp::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &ofxQuadWarp::keyReleased);
    }
    catch(Poco::SystemException) {
        return;
    }
}

void ofxQuadWarp::enableMouseControlsSrc() {
    if(bMouseEnabledSrc == true) {
        return;
    }
    if(bMouseEnabled == true){
      disableMouseControls();
    }
    bMouseEnabledSrc = true;
    ofAddListener(ofEvents().mouseMoved, this, &ofxQuadWarp::onMouseMovedSrc);
    ofAddListener(ofEvents().mousePressed, this, &ofxQuadWarp::onMousePressedSrc);
    ofAddListener(ofEvents().mouseDragged, this, &ofxQuadWarp::onMouseDraggedSrc);
    ofAddListener(ofEvents().mouseReleased, this, &ofxQuadWarp::onMouseReleasedSrc);
}

void ofxQuadWarp::disableMouseControlsSrc() {
    if(bMouseEnabledSrc == false) {
        return;
    }
    bMouseEnabledSrc = false;
    try {
        ofRemoveListener(ofEvents().mouseMoved, this, &ofxQuadWarp::onMouseMovedSrc);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxQuadWarp::onMousePressedSrc);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxQuadWarp::onMouseDraggedSrc);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxQuadWarp::onMouseReleasedSrc);
    }
    catch(Poco::SystemException) {
        return;
    }
}

void ofxQuadWarp::enableKeyboardShortcutsSrc() {
    if(bKeyboardShortcutsSrc == true) {
        return;
    }
    if(bKeyboardShortcuts == true){
      disableKeyboardShortcuts();
    }
    bKeyboardShortcutsSrc = true;
    ofAddListener(ofEvents().keyPressed, this, &ofxQuadWarp::keyPressedSrc);
    ofAddListener(ofEvents().keyReleased, this, &ofxQuadWarp::keyReleased);
}

void ofxQuadWarp::disableKeyboardShortcutsSrc() {
    if(bKeyboardShortcutsSrc == false) {
        return;
    }
    bKeyboardShortcutsSrc = false;
    try {
        ofRemoveListener(ofEvents().keyPressed, this, &ofxQuadWarp::keyPressedSrc);
        ofRemoveListener(ofEvents().keyReleased, this, &ofxQuadWarp::keyReleased);
    }
    catch(Poco::SystemException) {
        return;
    }
}

bool ofxQuadWarp::hitTest(ofVec2f pos) {
    // apply the inverse transformation to our hit point and then
    // compare to the input rectangle
    
    ofMatrix4x4 mat = getMatrix().getInverse();
    ofVec4f posTransformed = mat.preMult(ofVec3f(pos.x, pos.y, 0.0));
    float a2 = anchorSize * 0.5;
    if(posTransformed.x > srcPoints[0].x - a2 && posTransformed.y > srcPoints[0].y - a2 &&
       posTransformed.x < srcPoints[2].x + a2 && posTransformed.y < srcPoints[2].y + a2) {
        return true;
    }
    return false;
}

ofRectangle ofxQuadWarp::boundingBox() {
    ofRectangle res(dstPoints[0].x, dstPoints[0].y, 1, 1);
    res.growToInclude(dstPoints[1]);
    res.growToInclude(dstPoints[2]);
    res.growToInclude(dstPoints[3]);
    return res;
}

//----------------------------------------------------- source / target points.
void ofxQuadWarp::setSourceRect(const ofRectangle& r) {
    srcPoints[0].set(r.x, r.y);
    srcPoints[1].set(r.x + r.width, r.y);
    srcPoints[2].set(r.x + r.width, r.y + r.height);
    srcPoints[3].set(r.x, r.y + r.height);
}

void ofxQuadWarp::setTargetRect(const ofRectangle& r) {
    dstPoints[0].set(r.x, r.y);
    dstPoints[1].set(r.x + r.width, r.y);
    dstPoints[2].set(r.x + r.width, r.y + r.height);
    dstPoints[3].set(r.x, r.y + r.height);
}

void ofxQuadWarp::setTargetPoints(const vector<ofPoint>& points) {
    int t = std::max(4, static_cast<int>(points.size()));
    for(int i=0; i<t; i++) {
        dstPoints[i].set(points[i]);
    }
}

void ofxQuadWarp::setSourcePoints(const vector<ofPoint>& points) {
    int t = std::min(4, static_cast<int>(points.size()));
    for(int i=0; i<t; i++) {
        srcPoints[i].set(points[i]);
    }
}


vector<ofPoint> ofxQuadWarp::getTargetPoints() {
    vector<ofPoint> points(dstPoints, dstPoints + sizeof dstPoints / sizeof dstPoints[0]);
    return points;
}

vector<ofPoint> ofxQuadWarp::getSourcePoints() {
    vector<ofPoint> points(srcPoints, srcPoints + sizeof srcPoints / sizeof srcPoints[0]);
    return points;
}

//----------------------------------------------------- matrix.
ofMatrix4x4 ofxQuadWarp::getMatrix() const {
    return getMatrix(&srcPoints[0], &dstPoints[0]);
}

ofMatrix4x4 ofxQuadWarp::getMatrixInverse() const {
    return getMatrix(&dstPoints[0], &srcPoints[0]);
}

ofMatrix4x4 ofxQuadWarp::getMatrix(const ofPoint* srcPoints, const ofPoint* dstPoints) const {
    return ofxHomography::findHomography(srcPoints, dstPoints);
}

void ofxQuadWarp::reset() {
    dstPoints[0].set(srcPoints[0]);
    dstPoints[1].set(srcPoints[1]);
    dstPoints[2].set(srcPoints[2]);
    dstPoints[3].set(srcPoints[3]);
}

//----------------------------------------------------- interaction.
void ofxQuadWarp::onMouseMoved(ofMouseEventArgs& mouseArgs) {
    if(bShow == false) {
        return;
    }
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    for(int i=0; i<4; i++) {
        ofPoint & dstPoint = dstPoints[i];
        if(mousePoint.distance(dstPoint) <= anchorSize * 0.5) {
            highlightCornerIndex = i;
            return;
        }
    }
    highlightCornerIndex = -1;
}

void ofxQuadWarp::onMousePressed(ofMouseEventArgs& mouseArgs) {
    if(bShow == false) {
        return;
    }
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    for(int i=0; i<4; i++) {
        ofPoint & dstPoint = dstPoints[i];
        if(mousePoint.distance(dstPoint) <= anchorSize * 0.5) {
            dstPoint.set(mousePoint);
            selectedCornerIndex = i;
            return;
        }
    }
    selectedCornerIndex = -1;
}

void ofxQuadWarp::onMouseDragged(ofMouseEventArgs& mouseArgs) {
    if(bShow == false) {
        return;
    }
    if(selectedCornerIndex < 0) return; // no corner selected
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    dstPoints[selectedCornerIndex].set(mousePoint);
}

void ofxQuadWarp::onMouseReleased(ofMouseEventArgs& mouseArgs) {
    if(bShow == false) {
        return;
    }
    if(selectedCornerIndex < 0) return; // none selected
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    dstPoints[selectedCornerIndex].set(mousePoint);
}

void ofxQuadWarp::keyPressed(ofKeyEventArgs& keyArgs) {
    if(bShow == false) {
        return;
    }

    if(keyArgs.key == OF_KEY_SHIFT)
        bShiftPressed = true;

    if(keyArgs.key == 'p') { // select nxt pt
        selectedCornerIndex = (selectedCornerIndex + 1) % 4;
    }
    else if(keyArgs.key == 'o') { // select prev pt
        selectedCornerIndex = (selectedCornerIndex - 1);
        if(selectedCornerIndex < 0)
            selectedCornerIndex = 3;
    }
    
    if(selectedCornerIndex < 0) return; // no corner selected. only happens if we didn't select one using 'o'/'p' before
    
    float nudgeAmount = 0.25;
    if(bShiftPressed) nudgeAmount = 10;
    ofPoint & selectedPoint = dstPoints[selectedCornerIndex];
    
    switch (keyArgs.key) {
    case OF_KEY_LEFT:
        selectedPoint.x -= nudgeAmount;
        break;
    case OF_KEY_RIGHT:
        selectedPoint.x += nudgeAmount;
        break;
    case OF_KEY_UP:
        selectedPoint.y -= nudgeAmount;
        break;
    case OF_KEY_DOWN:
        selectedPoint.y += nudgeAmount;
        break;
    default:
        break;
    }
}

void ofxQuadWarp::keyReleased(ofKeyEventArgs& keyArgs) {
    if(keyArgs.key == OF_KEY_SHIFT)
        bShiftPressed = false;
}

//----------------------------------------------------- interaction for source points.
void ofxQuadWarp::onMouseMovedSrc(ofMouseEventArgs& mouseArgs) {
    if(bShowSrc == false) {
        return;
    }

    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    for(int i=0; i<4; i++) {
        ofPoint & srcPoint = srcPoints[i];
        if(mousePoint.distance(srcPoint) <= anchorSize * 0.5) {
            highlightCornerIndex = i;
            return;
        }
    }
    highlightCornerIndex = -1;
}

void ofxQuadWarp::onMousePressedSrc(ofMouseEventArgs& mouseArgs) {
    if(bShowSrc == false) {
        return;
    }

    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    for(int i=0; i<4; i++) {
        ofPoint & srcPoint = srcPoints[i];
        if(mousePoint.distance(srcPoint) <= anchorSize * 0.5) {
            srcPoint.set(mousePoint);
            selectedCornerIndex = i;
            return;
        }
    }
    selectedCornerIndex = -1;
}

void ofxQuadWarp::onMouseDraggedSrc(ofMouseEventArgs& mouseArgs) {
    if(bShowSrc == false) {
        return;
    }
    if(selectedCornerIndex < 0) return; // no corner selected

    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    srcPoints[selectedCornerIndex].set(mousePoint);
}

void ofxQuadWarp::onMouseReleasedSrc(ofMouseEventArgs& mouseArgs) {
    if(bShowSrc == false) {
        return;
    }
    if(selectedCornerIndex < 0) return; // none selected

    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    srcPoints[selectedCornerIndex].set(mousePoint);
}

void ofxQuadWarp::keyPressedSrc(ofKeyEventArgs& keyArgs) {
    if(bShowSrc == false) {
        return;
    }

    if(keyArgs.key == OF_KEY_SHIFT)
        bShiftPressed = true;

    if(keyArgs.key == 'p') { // select nxt pt
        selectedCornerIndex = (selectedCornerIndex + 1) % 4;
    }
    else if(keyArgs.key == 'o') { // select prev pt
        selectedCornerIndex = (selectedCornerIndex - 1);
        if(selectedCornerIndex < 0)
            selectedCornerIndex = 3;
    }

    if(selectedCornerIndex < 0) return; // no corner selected. only happens if we didn't select one using 'o'/'p' before

    float nudgeAmount = 0.25;
    if(bShiftPressed) nudgeAmount = 10;
    ofPoint & selectedPoint = srcPoints[selectedCornerIndex];

    switch (keyArgs.key) {
    case OF_KEY_LEFT:
        selectedPoint.x -= nudgeAmount;
        break;
    case OF_KEY_RIGHT:
        selectedPoint.x += nudgeAmount;
        break;
    case OF_KEY_UP:
        selectedPoint.y -= nudgeAmount;
        break;
    case OF_KEY_DOWN:
        selectedPoint.y += nudgeAmount;
        break;
    default:
        break;
    }
}

//----------------------------------------------------- show / hide.
void ofxQuadWarp::show() {
    if(bShow) return;
    if(bShowSrc) bShowSrc = false;
    toggleShow();
}

void ofxQuadWarp::showSrc() {
    if(bShowSrc) return;
    if(bShow) bShow = false;
    toggleShowSrc();
}

void ofxQuadWarp::hide() {
    if(!bShow) return;
    toggleShow();
}

void ofxQuadWarp::hideSrc() {
    if(!bShowSrc) return;
    toggleShowSrc();
}

void ofxQuadWarp::toggleShow() {
    bShow = !bShow;
}

void ofxQuadWarp::toggleShowSrc() {
    bShowSrc = !bShowSrc;
}

bool ofxQuadWarp::isShowing() {
    return bShow;
}

//----------------------------------------------------- save / load.
void ofxQuadWarp::save(const string& path) {
    ofXml xml;
    xml.addChild("quadwarp");
    xml.setTo("quadwarp");
    xml.addChild("src");
    xml.setTo("src");
    for(int i=0; i<4; i++) {
        xml.addChild("point");
        xml.setToChild(i);
        xml.setAttribute("x", ofToString(srcPoints[i].x));
        xml.setAttribute("y", ofToString(srcPoints[i].y));
        xml.setToParent();
    }
    xml.setToParent();
    xml.addChild("dst");
    xml.setTo("dst");
    for(int i=0; i<4; i++) {
        xml.addChild("point");
        xml.setToChild(i);
        xml.setAttribute("x", ofToString(dstPoints[i].x));
        xml.setAttribute("y", ofToString(dstPoints[i].y));
        xml.setToParent();
    }
    xml.setToParent();
    
    xml.setToParent();
    xml.save(path);
}


inline bool fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

void ofxQuadWarp::load(const string& path) {    
    ofXml xml;
    bool bOk = xml.load(path);
    if(bOk == false) {
        return;
    }
    
    bOk = xml.setTo("quadwarp");
    if(bOk == false) {
        return;
    }
    
    bOk = xml.setTo("src");
    if(bOk == false) {
        return;
    }
    
    for(int i=0; i<xml.getNumChildren(); i++) {
        bOk = xml.setToChild(i);
        if(bOk == false) {
            continue;
        }
        srcPoints[i].x = ofToFloat(xml.getAttribute("x"));
        srcPoints[i].y = ofToFloat(xml.getAttribute("y"));
        xml.setToParent();
    }
    xml.setToParent();
    
    bOk = xml.setTo("dst");
    if(bOk == false) {
        return;
    }
    
    for(int i=0; i<xml.getNumChildren(); i++) {
        bOk = xml.setToChild(i);
        if(bOk == false) {
            continue;
        }
        dstPoints[i].x = ofToFloat(xml.getAttribute("x"));
        dstPoints[i].y = ofToFloat(xml.getAttribute("y"));
        xml.setToParent();
    }
    xml.setToParent();
    xml.setToParent();
}

//----------------------------------------------------- show / hide.
void ofxQuadWarp::draw() {
  ofPushStyle();
    if(bShow){
      ofSetColor(ofColor::cyan);
      drawQuadOutline();

      ofSetColor(ofColor::yellow);
      drawCorners();

      ofSetColor(ofColor::magenta);
      drawHighlightedCorner();

      ofSetColor(ofColor::red);
      drawSelectedCorner();
    } else if(bShowSrc){
      ofSetColor(ofColor::cyan);
      drawQuadOutline(srcPoints);

      ofSetColor(ofColor::yellow);
      drawCorners(srcPoints);

      ofSetColor(ofColor::magenta);
      drawHighlightedCorner(srcPoints);

      ofSetColor(ofColor::red);
      drawSelectedCorner(srcPoints);
    }
    ofPopStyle();
}

void ofxQuadWarp::drawSrc() {

}

void ofxQuadWarp::drawQuadOutline() {
  drawQuadOutline(dstPoints);
}

void ofxQuadWarp::drawQuadOutline(ofPoint* points) {
    if((bShow || bShowSrc) == false) return;
    
    for(int i=0; i<4; i++) {
        int j = (i+1) % 4;
        ofDrawLine(points[i].x,
                   points[i].y,
                   points[j].x,
                   points[j].y);
    }
}

void ofxQuadWarp::drawCorners() {
  drawCorners(dstPoints);
}

void ofxQuadWarp::drawCorners(ofPoint* points) {
    if((bShow || bShowSrc) == false) return;

    for(int i=0; i<4; i++) {
        ofPoint & point = points[i];
        drawCornerAt(point);
    }
}

void ofxQuadWarp::drawHighlightedCorner() {
  drawHighlightedCorner(dstPoints);
}

void ofxQuadWarp::drawHighlightedCorner(ofPoint* points) {
    if((bShow || bShowSrc) == false) return;

    if(highlightCornerIndex < 0) return;

    ofPoint & point = points[highlightCornerIndex];
    drawCornerAt(point);
}

void ofxQuadWarp::drawSelectedCorner() {
  drawSelectedCorner(dstPoints);
}

void ofxQuadWarp::drawSelectedCorner(ofPoint* points) {
    if((bShow || bShowSrc) == false) return;

    if(selectedCornerIndex < 0) return;
    
    ofPoint & point = points[selectedCornerIndex];
    drawCornerAt(point);
}

void ofxQuadWarp::drawCornerAt(const ofPoint & point) {
    ofDrawRectangle(point.x - anchorSize * 0.5,
                    point.y - anchorSize * 0.5,
                    anchorSize, anchorSize);
}
