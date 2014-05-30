//
//  Content.cpp
//  tj_client
//
//  Created by Kenta Hara on 5/25/14.
//
//

#include "ofxGifDecoder.h"
#include "Content.h"

void Content::setup(ofFile f) {
    if (f.getExtension() == "gif") {
        type = TJ_CONTENT_GIF;
    } else if (f.getExtension() == "mp4" || "mov") {
        type = TJ_CONTENT_VID;
    } else if (f.getExtension() == "txt") {
        type = TJ_CONTENT_TEXT;
    } else {
        type = TJ_CONTENT_IMG;
    }
}

void Content::update() {
    switch (type) {
        case TJ_CONTENT_IMG:
            img.update();
            break;
        case TJ_CONTENT_VID:
            vid.update();
        default:
            break;
    }
}


// Standard drawing
void Content::draw(const ofPoint p) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.draw(p.x, p.y);
            break;
        case TJ_CONTENT_IMG:
            img.draw(p);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.draw(p);
        default:
            break;
    }
}
void Content::draw(int x, int y) {
    switch (type) {
        case TJ_CONTENT_GIF:

            break;
        case TJ_CONTENT_IMG:

        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:

        default:
            break;
    }
}
void Content::draw(int x, int y, int w, int h) {
    switch (type) {
        case TJ_CONTENT_GIF:
            ofPushMatrix();
                ofScale((float)w/gif.getWidth(), (float)h/gif.getHeight());
                gif.draw(x, y);
            ofPopMatrix();
            break;
        case TJ_CONTENT_IMG:
            img.draw(x, y, w, h);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.draw(x, y, w, h);
        default:
            break;
    }
}


// Drawing with frame
void Content::drawFrame(int frame, const ofPoint p) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.drawFrame(frame, p.x, p.y);
            break;
        case TJ_CONTENT_IMG:
            img.draw(p.x, p.y);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.setFrame(frame);
            vid.draw(p.x, p.y);
        default:
            break;
    }
}

void Content::drawFrame(int frame, int x, int y) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.drawFrame(frame, x, y);
            break;
        case TJ_CONTENT_IMG:
            img.draw(x, y);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.setFrame(frame);
            vid.draw(x, y);
        default:
            break;
    }
}

void Content::drawFrame(int frame, int x, int y, int w, int h) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.drawFrame(frame, x, y, w, h);
            break;
        case TJ_CONTENT_IMG:
            img.draw(x, y, w, h);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.setFrame(frame);
            vid.draw(x, y, w, h);
        default:
            break;
    }
}


// Drawing frame by percent
void Content::drawFrameByPercent(float rate, const ofPoint p) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.drawFrame(rate*gif.getNumFrames(), p.x, p.y);
            break;
        case TJ_CONTENT_IMG:
            img.draw(p.x, p.y);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.setFrame(rate*vid.getTotalNumFrames());
            vid.draw(p.x, p.y);
        default:
            break;
    }
}

void Content::drawFrameByPercent(float rate, int x, int y) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.drawFrame(rate*gif.getNumFrames(), x, y);
            break;
        case TJ_CONTENT_IMG:
            img.draw(x, y);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.setFrame(rate*vid.getTotalNumFrames());
            vid.draw(x, y);
        default:
            break;
    }
}

void Content::drawFrameByPercent(float rate, int x, int y, int w, int h) {
    switch (type) {
        case TJ_CONTENT_GIF:
            gif.drawFrame(rate*gif.getNumFrames(), x, y, w, h);
            break;
        case TJ_CONTENT_IMG:
            img.draw(x, y, w, h);
        case TJ_CONTENT_TEXT:
            // not_impl
        case TJ_CONTENT_VID:
            vid.setFrame(rate*vid.getTotalNumFrames());
            vid.draw(x, y, w, h);
        default:
            break;
    }
}