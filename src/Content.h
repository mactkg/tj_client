//
//  Content.h
//  tj_client
//
//  Created by Kenta Hara on 5/25/14.
//
//

#ifndef __tj_client__Content__
#define __tj_client__Content__

#include "ofxGifDecoder.h"
#include "ContentsManager.h"

enum TJ_CONTENT_TYPE {
    TJ_CONTENT_GIF,
    TJ_CONTENT_IMG,
    TJ_CONTENT_TEXT,
    TJ_CONTENT_VID
};

class Content {
public:    
    Content(){
        ready = false;
    };
    ~Content(){};
    
    void setup(ofFile f);
    void update();
    
    void draw(const ofPoint p);
    void draw(int x, int y);
    void draw(int x, int y, int w, int h);
    
    void drawFrame(int frame, const ofPoint p);
    void drawFrame(int frame, int x, int y);
    void drawFrame(int frame, int x, int y, int w, int h);
    
    void drawFrameByPercent(float rate, const ofPoint p);
    void drawFrameByPercent(float rate, int x, int y);
    void drawFrameByPercent(float rate, int x, int y, int w, int h);
    
    void setRhythm(int _rhythm) {rhythm = _rhythm;};
    int getRhythm() {return rhythm;};
    
    int frameNum() {
        switch (type) {
            case TJ_CONTENT_GIF:
                return gif.getNumFrames();
            case TJ_CONTENT_IMG:
                return 1;
            case TJ_CONTENT_TEXT:
                return 1;
            case TJ_CONTENT_VID:
                return vid.getTotalNumFrames();
            default:
                -1;
        }
    };
    
    bool isReady() {
        return ready;
    }
    
    TJ_CONTENT_TYPE type;
    string filename;
    bool ready;

//private:
    ofxGifFile gif;
    ofImage img;
    ofVideoPlayer vid;
    string text;
    ofTrueTypeFont font;
    
    int rhythm;
};

#endif /* defined(__tj_client__Content__) */
