#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // opengl sharing
    // ofxOpenGLContextScope::setup();
    
    // Kinect
#ifdef USE_KINECT
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    if (kinect.isConnected()) {
        ofLogNotice() << "Kinect connected!";
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
#endif
    
    // Manage BPM
    bpm.start();
    
    // Contents Manager
    cm.setup(10, 1000);
    cm.setMaxContentsNum(7);
    
    // MIDI
    midiIn.listPorts();
    midiIn.openPort("nanoKONTROL2 SLIDER/KNOB");
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    
    // Projector
    projector.setup("Projector", -1280, -20, 1280, 820, true);
    //projector.setup("Projector", 0, 0, 800, 800, true);
    projector.begin();
    ofBackground(48, 77, 106);
    projector.end();
    
    // Font
    font.loadFont("fonts/Futura.ttc", 20);
    
    // Graphics
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(48, 77, 106);
}

//--------------------------------------------------------------
void ofApp::update(){
    // Kinect
#ifdef USE_KINECT
    kinect.update();
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
    projector.begin();
    {
#ifdef USE_KINECT
        easyCam.begin();
            drawPointCloud();
        easyCam.end();
#endif
        for (auto gc = cm.gifs.begin(); gc != cm.gifs.end(); gc++) {
            ofxThreadedGifFile &gif = gc->getGif();
            float s = 0;
            if (gif.getHeight() < gif.getWidth())
                s = (float)projector.getWidth()/gif.getWidth();
            else
                s = (float)projector.getHeight()/gif.getHeight();
            int x = gif.getWidth()*s;
            int y = gif.getHeight()*s;
            gif.drawFrame((int)(gif.getNumFrames()*bpm.getBeatByPercent()*gc->getPlayScale())%gif.getNumFrames(),
                              (projector.getWidth()-x)/2, (projector.getHeight()-y)/2, x, y);
        }
    }
    projector.end();
    
    
    // backyard
    font.drawString("Backyard - BPM:" + ofToString(bpm.getBpm(), 2), 10, 30);
    ofNoFill();
    ofRect(340, 9, 200, 20);
    ofFill();
    ofRect(340, 9, 200*bpm.getBeatByPercent(), 20);
    
    int x = 0;
    for (auto gc = cm.gifs.begin(); gc != cm.gifs.end(); gc++) {
        gc->drawBackyard(x*150+10, 600);
        x++;
    }
}

void ofApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.setCameraTiltAngle(0);
    kinect.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        bpm.tap();
    } else if (key == 'r') {
        bpm.reset();
    } else if (key == OF_KEY_DOWN) {
        bpm.setBpm(bpm.getBpm()-0.1);
    } else if (key == OF_KEY_UP) {
        bpm.setBpm(bpm.getBpm()+0.1);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg){
    
}