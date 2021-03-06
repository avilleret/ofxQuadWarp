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

    srcPoints.resize(4);
    dstPoints.resize(4);
    
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
    if (bNormalize)
    {
        for(int i=0; i<t; i++) {
            dstPoints[i].set(points[i] / ofGetWindowSize());
        }
    } else {
        for(int i=0; i<t; i++) {
            dstPoints[i].set(points[i]);
        }
    }
}

void ofxQuadWarp::setSourcePoints(const vector<ofPoint>& points) {
    int t = std::min(4, static_cast<int>(points.size()));
    if (bNormalize)
    {
        for(int i=0; i<t; i++) {
            srcPoints[i].set(points[i] / ofGetWindowSize());
        }
    } else {
        for(int i=0; i<t; i++) {
            srcPoints[i].set(points[i]);
        }
    }
}

std::vector<ofPoint>& ofxQuadWarp::getTargetPoints() {
   return dstPoints;
}

std::vector<ofPoint>& ofxQuadWarp::getSourcePoints() {
  return srcPoints;
}

//----------------------------------------------------- matrix.
ofMatrix4x4 ofxQuadWarp::getMatrix() const {
  return getMatrix(srcPoints,dstPoints);
}

ofMatrix4x4 ofxQuadWarp::getMatrixScaled(
    ofVec3f src_scale, ofVec3f dst_scale) const
{
    auto src = srcPoints;
    auto dst = dstPoints;

    if (bNormalize)
    {
        for (auto& p : src)
            p *= src_scale;
        for (auto& p : dst)
            p *= dst_scale;
    }

    return getMatrix(src,dst);
}

ofMatrix4x4 ofxQuadWarp::getMatrixInverse() const {
    return getMatrix(dstPoints, srcPoints);
}

ofMatrix4x4 ofxQuadWarp::getMatrix(const std::vector<ofPoint>& srcPoints, const std::vector<ofPoint>& dstPoints) const {
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
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    float threshold = anchorSize * 0.5;

    if (bNormalize){
        mousePoint /= ofGetWindowSize();
        threshold /= ofGetWidth();
    }

    for(int i=0; i<4; i++) {
      ofPoint* pt{};

        if(bShow) {
            pt = &dstPoints[i];
        } else if (bShowSrc) {
            pt = &srcPoints[i];
        } else {
            return;
        }
        if(mousePoint.distance(*pt) <= threshold) {
            highlightCornerIndex = i;
            return;
        }
    }
    highlightCornerIndex = -1;
}

void ofxQuadWarp::onMousePressed(ofMouseEventArgs& mouseArgs) {

    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    float threshold = anchorSize * 0.5;

    if (bNormalize){
        mousePoint /= ofGetWindowSize();
        threshold /= ofGetWidth();
    }

    for(int i=0; i<4; i++) {
        ofPoint* pt{};
        if(bShow) {
            pt = &dstPoints[i];
        } else if (bShowSrc) {
            pt = &srcPoints[i];
        } else {
          return;
        }

        if(mousePoint.distance(*pt) <= threshold) {
            pt->set(mousePoint);
            selectedCornerIndex = i;
            return;
        }
    }
    selectedCornerIndex = -1;
}

void ofxQuadWarp::onMouseDragged(ofMouseEventArgs& mouseArgs) {

    if(selectedCornerIndex < 0) return; // no corner selected
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);

    if (bNormalize)
        mousePoint /= ofGetWindowSize();

    if(bShow) {
        dstPoints[selectedCornerIndex].set(mousePoint);
    } else if (bShowSrc) {
        srcPoints[selectedCornerIndex].set(mousePoint);
    }
}

void ofxQuadWarp::onMouseReleased(ofMouseEventArgs& mouseArgs) {

    if(selectedCornerIndex < 0) return; // none selected
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);

    if (bNormalize)
        mousePoint /= ofGetWindowSize();

    if(bShow) {
        dstPoints[selectedCornerIndex].set(mousePoint);
    } else if (bShowSrc) {
        srcPoints[selectedCornerIndex].set(mousePoint);
    }
}

void ofxQuadWarp::keyPressed(ofKeyEventArgs& keyArgs) {

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
    if (bNormalize)
        nudgeAmount /= ofGetWidth();

    ofPoint* selectedPoint{};

    if(bShow) {
        selectedPoint = &dstPoints[selectedCornerIndex];
    } else if (bShowSrc) {
        selectedPoint = &srcPoints[selectedCornerIndex];
    } else {
        return;
    }

    switch (keyArgs.key) {
    case OF_KEY_LEFT:
        selectedPoint->x -= nudgeAmount;
        break;
    case OF_KEY_RIGHT:
        selectedPoint->x += nudgeAmount;
        break;
    case OF_KEY_UP:
        selectedPoint->y -= nudgeAmount;
        break;
    case OF_KEY_DOWN:
        selectedPoint->y += nudgeAmount;
        break;
    default:
        break;
    }
}

void ofxQuadWarp::keyReleased(ofKeyEventArgs& keyArgs) {
    if(keyArgs.key == OF_KEY_SHIFT)
        bShiftPressed = false;
}

//----------------------------------------------------- show / hide.
void ofxQuadWarp::show() {
    if(bShow)
      return;

    if(bShowSrc)
      bShowSrc = false;

    toggleShow();
}

void ofxQuadWarp::showSrc() {
    if(bShowSrc)
      return;

    if(bShow)
      bShow = false;

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
  std::vector<ofPoint> points;

  if(bShow)
  {
    points = dstPoints;
  }
  else if (bShowSrc)
  {
    points = srcPoints;
  }

  if (bNormalize)
  {
    for (auto& p : points)
      p *= ofGetWindowSize();
  }
  ofSetColor(ofColor::cyan);
  drawQuadOutline(points);

  ofSetColor(ofColor::yellow);
  drawCorners(points);

  ofSetColor(ofColor::magenta);
  drawHighlightedCorner(points);

  ofSetColor(ofColor::red);
  drawSelectedCorner(points);

  ofPopStyle();
}

void ofxQuadWarp::drawQuadOutline() {
  drawQuadOutline(dstPoints);
}

void ofxQuadWarp::drawQuadOutline(const std::vector<ofPoint>& points) {
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

void ofxQuadWarp::drawCorners(const std::vector<ofPoint>& points) {
    if((bShow || bShowSrc) == false) return;

    for(auto p : points) {
        drawCornerAt(p);
    }
}

void ofxQuadWarp::drawHighlightedCorner() {
  drawHighlightedCorner(dstPoints);
}

void ofxQuadWarp::drawHighlightedCorner(const std::vector<ofPoint>& points) {
    if((bShow || bShowSrc) == false) return;

    if(highlightCornerIndex < 0) return;

    const ofPoint & point = points[highlightCornerIndex];
    drawCornerAt(point);
}

void ofxQuadWarp::drawSelectedCorner() {
  drawSelectedCorner(dstPoints);
}

void ofxQuadWarp::drawSelectedCorner(const std::vector<ofPoint>& points) {
    if((bShow || bShowSrc) == false) return;

    if(selectedCornerIndex < 0) return;
    
    const ofPoint & point = points[selectedCornerIndex];
    drawCornerAt(point);
}

void ofxQuadWarp::drawCornerAt(const ofPoint & point) {
    ofDrawRectangle(point.x - anchorSize * 0.5,
                    point.y - anchorSize * 0.5,
                    anchorSize, anchorSize);
}
