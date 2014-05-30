#pragma once

#include "ofxThreadedGIFLoader.h"
#include "ofxPostProcessing.h"
#include "ofxMidi.h"
#include "ofxKinect.h"
#include "ofxFX.h"
#include "ofxPostProcessing.h"
#include "ofxBpm.h"
#include "ofxSecondWindow.h"
#include "ContentsManager.h"
//#include "ofxOpenGLContextScope.h"
#include "ofMain.h"

//#define USE_KINECT

class ofApp : public ofBaseApp, public ofxMidiListener {

	public:
		void setup();
		void update();
		void draw();
        void drawPointCloud();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void gifLoaded(ofxGifLoadedEvent &e);
    void newMidiMessage(ofxMidiMessage &msg);
    
    ofxSecondWindow projector;
    ofxThreadedGIFLoader gifLoader;
    ofxBpm bpm;
    ofxKinect kinect;
    ofEasyCam easyCam;
    ofTrueTypeFont font;
    ContentsManager cm;
    ofxMidiIn midiIn;
};
