#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofSetCircleResolution(60);
    ofSetFullscreen(true);
    /* --shaders*/
    #ifdef TARGET_OPENGLES
    shader.load("shadersES2/shader");
    #else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
    }else{
        shader.load("shadersGL2/shader");
    }
    #endif
    
    /*-----------------Vars setup --------------------------*/
    //particles
    currentMode = SNOW;
    force.set(0, 0.2, 0);
    bg_color = ofColor(0,55,110);
    noiseMagnitude = 0.8;
    timeFrequency = 0.5;
    ofBackground(bg_color);
    
    /*---------------- initialize origin---------------- */
    system.setup(ofVec3f(0, 0, 300), 100);
    
    /*-----------------GUI Setup----------------------------*/
    vector<string> forces_available;
    gui = new ofxUISuperCanvas("My Particles");
    gui->addTextArea("TEXT AREA", "f: Toggle Fullscreen", OFX_UI_FONT_SMALL);
    gui->addTextArea("TEXT AREA", "g: Hide/show the GUI panel.", OFX_UI_FONT_SMALL);
    gui->addTextArea("TEXT AREA", "<space>: Take Screenshot", OFX_UI_FONT_SMALL);
    gui->addTextArea("TEXT AREA", "<1>, <2> to change the force applied to the particles", OFX_UI_FONT_SMALL);
    gui->addSpacer();
    gui->addSlider("ZOOM", 50.00, 1000.00, zoom);
    gui->addSpacer();
    gui->addTextArea("Variables:", "Noise", OFX_UI_FONT_SMALL);
    gui->addSlider("Random Offset",	0, 5, randomOffset);
    gui->addSlider("Space Frequency",   0.0001f,  1.0f, spaceFrequency);
    gui->addSlider("Time Frequency",	0.01f,  2.0f, timeFrequency);
    gui->addSlider("Noise Magnitude",	0.1f,  2.0f, noiseMagnitude);
    gui->addSlider("Old Vel Amount",	0.2, 1, oldVelAmount);
    gui->addSlider("Max Vel", 1.0, 3.0, maxVel);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
}

//--------------------------------------------------------------
void ofApp::update(){
//    for (ParticleSystem system : systems){
//        system.setMode(currentMode);
//        system.update(force);
//    }
    //system.resetForces();
    
    switch (currentMode) {
        case NOISE:
            system.update(force, randomOffset, spaceFrequency, timeFrequency, noiseMagnitude, oldVelAmount);
            break;
        case SNOW:
            system.updateSnow();
            break;
            
        default:
            return 1;
            //system.update(force, randomOffset, spaceFrequency, timeFrequency, noiseMagnitude, oldVelAmount);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    camera.begin();
        // draw particles
        shader.begin();
            system.draw();
        shader.end();
    camera.end();
    ofDisableDepthTest();
    ofEnableBlendMode( OF_BLENDMODE_ALPHA );
    ofSetColor(230);
    ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 300, 20);
    
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    

    if (name == "ZOOM"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        zoom = rslider->getScaledValue();
        camera.setDistance(zoom);
    }
    else if(name == "FORCES"){
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        selected_force = radio->getValue();
    }
    else if(name == "Random Offset"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        randomOffset = rslider->getScaledValue();
    }
    else if(name == "Space Frequency"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        spaceFrequency = rslider->getScaledValue();
    }
    else if(name == "Time Frequency"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        timeFrequency = rslider->getScaledValue();
    }
    else if(name == "Noise Magnitude"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        noiseMagnitude = rslider->getScaledValue();
    }
    else if(name == "Old Vel Amount"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        oldVelAmount = rslider->getScaledValue();
    }
    else if(name == "Max Vel"){
        ofxUISlider *rslider = (ofxUISlider *) e.widget;
        maxVel = rslider->getScaledValue();
    }

}

void ofApp::screenshot(){
    ofImage screenshot;
    screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    string filename = "scrshot-" + ofGetTimestampString() + ".png";
    screenshot.saveImage(filename);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1':
            currentMode = NOISE;
            currentModeStr = "1 NOISE";
            break;
        case '2':
            currentMode = TRIGO;
            currentModeStr = "2 Trigo";
            break;
        case '3':
            currentMode = SNOW;
            currentModeStr = "3 SNOW";
            break;
        case 'f':
            ofToggleFullscreen();
            break;
            
        case 'g':
            gui->toggleVisible();
            break;
            
        case 'p':
            draw_padding = !draw_padding;
            gui->setDrawWidgetPadding(draw_padding);
            //            gui->setDrawPadding(drawPadding);
            //            gui->setDrawWidgetPaddingOutline(drawPadding);
            break;

        case ' ':
        {
            ofImage screenshot;
            screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            //rename it according to the current timestamp
            string filename = "scrshot-" + ofGetTimestampString() + ".png";
            screenshot.saveImage(filename);
            break;
        }
            
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::resetForcesParticleSystems(){
    system.resetForces();
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
