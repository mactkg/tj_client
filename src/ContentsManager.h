//
//  ContentsManager.h
//  tj_client
//
//  Created by Kenta Hara on 5/25/14.
//
//

#ifndef __tj_client__ContentsManager__
#define __tj_client__ContentsManager__

#include <vector>
#include "ofxThreadedGIFLoader.h"
#include "ofMain.h"

/*struct GifContent {
    string path;
    ofxThreadedGifFile gif;
    ofPoint interfacePoint;
    ofPoint projectionPoint;
    float playScale;
};*/

class GifContent {
public:
    GifContent();
    ~GifContent(){};
    void setup(string path, ofxThreadedGifFile &gif);
    void draw(int frame, int x, int y, int w, int h);
    void drawBackyard(int x, int y);
    
    string getPath() { return path; }
    ofxThreadedGifFile & getGif() { return gif; };
    float getPlayScale() { return playScale; };
    
    void mousePressed(ofMouseEventArgs &e);
    void reset();
    
    bool isLocked;
    bool isDeleted;
    int value;
    float displayScale;
    
private:
    string path;
    ofxThreadedGifFile gif;
    ofPoint interfacePoint;
    ofPoint projectionPoint;
    float playScale;
};

class ContentsManager {
public:
    //ContentsManager(){};
    //~ContentsManager(){};
    
    void setup(int maxContentsNum, int interval);
    
    void update(ofEventArgs &e);
    void gifLoaded(ofxGifLoadedEvent &e);
    
    void setMaxContentsNum(int num) { maxContentsNum = num; };
    int getMaxContentsNum() { return maxContentsNum; };

    void setIntervalTime(int t) { intervalTime = t; };
    void getIntervalTime() { return intervalTime; };
    
    void setSharedFont(ofTrueTypeFont &sharedFont) { font = sharedFont; };
    void mousePressed(ofMouseEventArgs &e);
    
    vector<GifContent> gifs;
    //std::map<string, GifContent> gifs;
    std::map<string, ofImage> imgs;
    ofxThreadedGIFLoader gifloader;
    static ofTrueTypeFont font;

private:
    int maxContentsNum;
    int intervalTime;
    unsigned long long lastUpdateTime;
    
};

#endif /* defined(__tj_client__ContentsManager__) */
