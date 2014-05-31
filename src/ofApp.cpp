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
    projector.setup("Projector", -640, -20, 640, 480, true);
    //projector.setup("Projector", 0, 0, 800, 800, true);
    projector.begin();
    ofBackground(48, 77, 106);
    projector.end();
    
    // FBO
    fbo.allocate(projector.getWidth(), projector.getHeight());
    fbo.begin();
    ofColor(0, 0, 0, 255);
    fbo.end();
    
    // Glitch
    glitch.setup(&fbo);
    
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

    for (int i = 0; i < 17; i++) {
        if (goGlitch[i]) {
            glitch.setFx(ofxPostGlitchType(i), true);
        } else {
            glitch.setFx(ofxPostGlitchType(i), false);
        }
    }
    
    fbo.begin();
    ofClear(TUMBLR_BACKGROUND);
    for (auto gc = cm.gifs.begin(); gc != cm.gifs.end(); gc++) {
        ofxThreadedGifFile &gif = gc->getGif();
        if (gc->value == 0) {
            continue;
        }
        float s = 0;
        if (gif.getHeight() < gif.getWidth())
            s = (float)projector.getWidth()/gif.getWidth()*gc->displayScale;
        else
            s = (float)projector.getHeight()/gif.getHeight()*gc->displayScale;
        int x = gif.getWidth()*s;
        int y = gif.getHeight()*s;
        
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, gc->value*2);
        gif.drawFrame((int)(gif.getNumFrames()*bpm.getBeatByPercent()*gc->getPlayScale())%gif.getNumFrames(),
                      (projector.getWidth()-x)/2, (projector.getHeight()-y)/2, x, y);
        ofDisableAlphaBlending();
    }
    fbo.end();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    projector.begin();
    {
        ofBackground(48, 77, 106);

#ifdef USE_KINECT
        easyCam.begin();
            drawPointCloud();
        easyCam.end();
#endif
        glitch.generateFx();
        fbo.draw(0, 0);
    }
    projector.end();
    
    
    // backyard
    ofSetColor(255);
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
    if (midiIn.isOpen()) {
        ofSetColor(0, 255, 0);
    } else ofSetColor(255, 0, 0);
    ofRect(ofGetWidth()-20, 0, 20, 20);
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
    midiIn.closePort();
    midiIn.removeListener(this);
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
    
    if (key == '1') goGlitch[OFXPOSTGLITCH_CONVERGENCE] = !goGlitch[OFXPOSTGLITCH_CONVERGENCE];
	if (key == '2') goGlitch[OFXPOSTGLITCH_GLOW] = !goGlitch[OFXPOSTGLITCH_GLOW];
	if (key == '3') goGlitch[OFXPOSTGLITCH_SHAKER] = !goGlitch[OFXPOSTGLITCH_SHAKER];
	if (key == '4') goGlitch[OFXPOSTGLITCH_CUTSLIDER] = !goGlitch[OFXPOSTGLITCH_CUTSLIDER];
	if (key == '5') goGlitch[OFXPOSTGLITCH_TWIST] = !goGlitch[OFXPOSTGLITCH_TWIST];
	if (key == '6') goGlitch[OFXPOSTGLITCH_OUTLINE] = !goGlitch[OFXPOSTGLITCH_OUTLINE];
	if (key == '7') goGlitch[OFXPOSTGLITCH_NOISE] = !goGlitch[OFXPOSTGLITCH_NOISE];
	if (key == '8') goGlitch[OFXPOSTGLITCH_SLITSCAN] = !goGlitch[OFXPOSTGLITCH_SLITSCAN];
	if (key == '9') goGlitch[OFXPOSTGLITCH_SWELL] = !goGlitch[OFXPOSTGLITCH_SWELL];
	if (key == '0') goGlitch[OFXPOSTGLITCH_INVERT] = !goGlitch[OFXPOSTGLITCH_INVERT];
    
	if (key == '-') goGlitch[OFXPOSTGLITCH_CR_HIGHCONTRAST] = !goGlitch[OFXPOSTGLITCH_CR_HIGHCONTRAST];
	if (key == '=') goGlitch[OFXPOSTGLITCH_CR_BLUERAISE] = !goGlitch[OFXPOSTGLITCH_CR_BLUERAISE];
	if (key == 'q') goGlitch[OFXPOSTGLITCH_CR_REDRAISE] = !goGlitch[OFXPOSTGLITCH_CR_REDRAISE];
	if (key == 'w') goGlitch[OFXPOSTGLITCH_CR_GREENRAISE] = !goGlitch[OFXPOSTGLITCH_CR_GREENRAISE];
	if (key == 'e') goGlitch[OFXPOSTGLITCH_CR_BLUEINVERT] = !goGlitch[OFXPOSTGLITCH_CR_BLUEINVERT];
	if (key == 't') goGlitch[OFXPOSTGLITCH_CR_REDINVERT] = !goGlitch[OFXPOSTGLITCH_CR_REDINVERT];
	if (key == 'y') goGlitch[OFXPOSTGLITCH_CR_GREENINVERT] = !goGlitch[OFXPOSTGLITCH_CR_GREENINVERT];
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
    ofLogNotice("MIDI") << msg.control;
    if (msg.control >= 64 && msg.control <= 71 &&
        msg.control <= cm.getMaxContentsNum()+64 && msg.value > 0) { // pushed 'R' button
        GifContent &gif = cm.gifs.at(msg.control-64);
        gif.isLocked = !gif.isLocked;
    } else if (msg.control >= 0 && msg.control <= 7 &&
               msg.control <= cm.getMaxContentsNum()) {
        GifContent &gif = cm.gifs.at(msg.control);
        gif.value = msg.value;
    } else if (msg.control >= 16 && msg.control <= 23 &&
               msg.control <= cm.getMaxContentsNum()+16) {
        cm.gifs.at(msg.control-16).displayScale = msg.value/127.0*2;
    }
}