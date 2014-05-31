//
//  ContentsManager.cpp
//  tj_client
//
//  Created by Kenta Hara on 5/25/14.
//
//

#include "ContentsManager.h"

ofTrueTypeFont ContentsManager::font;

void ContentsManager::setup(int maxContentsNum, int interval) {
    setIntervalTime(interval);
    setMaxContentsNum(maxContentsNum);
    lastUpdateTime = ofGetElapsedTimeMillis();
    
    ContentsManager::font.loadFont("fonts/Futura.ttc", 16);
    
    ofAddListener(ofEvents().update, this, &ContentsManager::update);
    ofAddListener(ofEvents().mousePressed, this, &ContentsManager::mousePressed);
    ofAddListener(ofxGifLoadedEvent::events, this, &ContentsManager::gifLoaded);
    gifloader.startThread();
}

void ContentsManager::update(ofEventArgs &e) {
    // regulary update
    if (ofGetElapsedTimeMillis() - lastUpdateTime > intervalTime) {
        lastUpdateTime = ofGetElapsedTimeMillis();
        
        ofDirectory dir;
        dir.listDir("contents");
        vector<ofFile> files = dir.getFiles();

        // cleaning
        int cnt = 0;
        for (auto gif = gifs.begin(); gif != gifs.end();) {
            bool found = false;
            for (auto file = files.begin(); file != files.end(); file++) {
                // alive!
                if (gif->getPath() == file->path()) {
                    files.erase(file);
                    found = true;
                    break;
                }
            }
            
            if(!found && !(gif->isLocked)) {
                gif->isDeleted = true;
            } else {
                cnt++;
            }
            ++gif;
        }
        
        // adding
        for (auto file = files.begin(); file != files.end(); file++) {
            if (file->getExtension() == "gif") {
                //gif here
                if (cnt < maxContentsNum) {
                    ofLogNotice("Loading Request") << file->path();
                    gifloader.loadFromDisk(file->path());
                    cnt++;
                }
            } else if (file->getExtension() == "txt") {
                //txt here
            } else {
                //img here
            }
        }
    }
}

void ContentsManager::gifLoaded(ofxGifLoadedEvent &e) {
    ofLogNotice() << "gif loaded!" << e.path;
    GifContent gc;
    gc.setup(e.path, e.gif);
    if (gifs.size() < maxContentsNum) {
        gifs.push_back(gc);
    } else {
        for (auto gif = gifs.begin(); gif != gifs.end(); gif++) {
            if (gif->isDeleted) {
                gifs.at(distance(gifs.begin(), gif)) = gc;
            }
        }
    }
}

void ContentsManager::mousePressed(ofMouseEventArgs &e) {
    for (auto it = gifs.begin(); it != gifs.end(); it++) {
        it->mousePressed(e);
    }
}

//****************************************************
// GifContent
//****************************************************

GifContent::GifContent() {
    path = "";
    interfacePoint = ofPoint(0, 0);
    projectionPoint = ofPoint(0, 0);
    playScale = 2;
    value = 0;
    displayScale = 1.0;
    isLocked = false;
    isDeleted = false;
    ofLogNotice() << "hello";
}

void GifContent::reset() {
    playScale = 2;
    value = 0;
    displayScale = 1.0;
}

void GifContent::setup(string pathName, ofxThreadedGifFile &gifFile) {
    path = pathName;
    gif = gifFile;
    //ofAddListener(ofEvents().mousePressed, this, &GifContent::mousePressed);
}

void GifContent::drawBackyard(int x, int y) {
    interfacePoint.x = x;
    interfacePoint.y = y;
    for (int i = 0; i < 5; i++) {
        ofRectangle r(x, 540-i*60, 40, 40);
        
        ofFill();
        if (ofGetMouseX() >= r.getLeft() && ofGetMouseX() <= r.getRight() &&
            ofGetMouseY() >= r.getTop()  && ofGetMouseY() <= r.getBottom()) {
            ofSetColor(149, 163, 176);
        } else {
            ofSetColor(86, 107, 129);
        }
        ofRect(r);
        ofSetColor(255);
        string str;
        bool selected = false;
        switch (i) {
            case 0:
                str = "x1/4";
                if (playScale <= 0.25)
                    selected = true;
                break;
            case 1:
                str = "x1/2";
                if (playScale <= 0.5 && playScale > 0.25)
                    selected = true;
                break;
            case 2:
                str = "x1";
                if (playScale <= 1.0 && playScale > 0.5)
                    selected = true;
                break;
            case 3:
                str = "x2";
                if (playScale <= 2.0 && playScale > 1.0)
                    selected = true;
                break;
            case 4:
                str = "x4";
                if (playScale >= 4.0)
                    selected = true;
                break;
            default:
                break;
        }
        ContentsManager::font.drawString(str, r.getRight()+4, r.getCenter().y+10);
        ContentsManager::font.drawString(ofToString(displayScale, 3), x, y+120);
        if (selected) {
            ofSetColor(220);
            ofLine(r.getTopLeft(), r.getBottomRight());
            ofLine(r.getTopRight(), r.getBottomLeft());
        }
    }
    
    ofFill();
    if (isLocked) {
        ofSetColor(255, 100, 100);
    } else {
        ofSetColor(255);
    }
    ofRect(x, y, 100, 100); // edge
    
    ofSetColor(255);
    ofRect(x+106, y+100-value/127.0*100, 10, value/127.0*100);
    if (!isDeleted) {
        gif.drawFrame(0, x+1, y+1, 98, 98);
    }
    x++;
}

void GifContent::mousePressed(ofMouseEventArgs &e) {
    for (int i = 0; i < 5; i++) {
        ofRectangle r(interfacePoint.x, 540-i*60, 40, 40);
        vector<ofPoint> p;
        p.push_back(r.getBottomLeft());
        p.push_back(r.getBottomRight());
        p.push_back(r.getTopLeft());
        p.push_back(r.getTopRight());
        if (e.x >= r.getLeft() && e.x <= r.getRight() &&
            e.y >= r.getTop()  && e.y <= r.getBottom()) {
            ofLogNotice() << "inside";
            float buf = 0.25;
            for (int n = i; n > 0; n--) {
                buf *= 2;
            }
            ofLogNotice() << buf;
            playScale = buf;
        }
    }
}