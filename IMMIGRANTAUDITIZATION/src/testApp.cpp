#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	 

	// load in sounds:
	beat.loadSound("sounds/jdee_beat.mp3");
	ow.loadSound("sounds/ow.mp3");	
	dog.loadSound("sounds/dog.mp3");	
	rooster.loadSound("sounds/rooster.mp3");
	
	// load in city imgs
	baltimore.load("imgs/baltimore.jpg");
    birmingham.load("imgs/birmingham.jpg");
    boston.load("imgs/boston.jpg");
    detroit.load("imgs/detroit.jpg");
    indianapolis.load("imgs/indianapolis.jpg");
    losangeles.load("imgs/los-angeles.jpg");
    nashville.load("imgs/nashville.jpg");
    washingtondc.load("imgs/washington-dc.jpg");
	
	// we will bounce a circle using these variables:
	px = 300;
	py = 300;
	vx = 0;
	vy = 0;	

	// the fft needs to be smoothed out, so we create an array of floats
	// for that purpose:
	fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = 128;
    
    //UI
    ofSetVerticalSync(true);
	ofEnableSmoothing();
    
    red = 233; blue = 27; green = 52;
    float dim = 32;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
	
    drawPadding = false;
    gui = new ofxUICanvas(0,0,length+xInit,ofGetHeight());
    
    gui->addWidgetDown(new ofxUILabel("Data Sound Toy", OFX_UI_FONT_LARGE));
    gui->addSpacer(length, 2);
    //year
    vector<string> hnames; hnames.push_back("2000"); hnames.push_back("2005"); hnames.push_back("2010");
	gui->addWidgetDown(new ofxUIRadio(dim, dim, "Year", hnames, OFX_UI_ORIENTATION_HORIZONTAL));
    //city
    gui->addSpacer(length-xInit, 2);
//    gui->addLabelButton("LABEL BUTTON", false);
//    gui->addWidgetEastOf(new ofxUILabel("<- Automagically Sized", OFX_UI_FONT_SMALL),"LABEL BUTTON");
//    gui->addWidgetSouthOf(new ofxUILabelButton(length-xInit, false, "SPECIFIED WIDTH BUTTON", OFX_UI_FONT_MEDIUM), "LABEL BUTTON");
    gui->addLabelToggle("NEW YORK", false);
    gui->addLabelToggle("BOSTON", false);
    

    //sound clips
    gui->addSpacer(length-xInit, 2);
    vector<string> vnames; vnames.push_back("ROCKS"); vnames.push_back("MY"); vnames.push_back("SOCKS");
   
    ofxUIRadio *radio = (ofxUIRadio *) gui->addWidgetDown(new ofxUIRadio(dim, dim, "VERTICAL RADIO", vnames, OFX_UI_ORIENTATION_VERTICAL));
    
    
    radio->activateToggle("SOCKS");
    
    gui->addSpacer(length-xInit, 2);
    gui->addWidgetDown(new ofxUIToggleMatrix(dim, dim, 3, 3, "3X3 MATRIX"));
    gui->addWidgetRight(new ofxUIToggleMatrix(dim, dim, 3, 3, "DISABLE MULTIPLE"));
    
    ofxUIToggleMatrix* mtx = (ofxUIToggleMatrix *)gui->getWidget("DISABLE MULTIPLE");
    mtx->setAllowMultiple(false);
    gui->addWidgetSouthOf(new ofxUIToggleMatrix(dim,dim,1,6,"CUSTOM SIZE"), "3X3 MATRIX");
    
    gui->addSpacer(length-xInit, 2);
    gui->addWidgetDown(new ofxUILabel("IMAGE BUTTON & TOGGLE", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUIImageButton(dim*2, dim*2, false, "GUI/pause.png", "IMAGE BUTTON"));
    gui->addWidgetRight(new ofxUIImageToggle(dim*2, dim*2, false, "GUI/play.png", "IMAGE TOGGLE"));
    
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
	ofBackground(red, green, blue);
}


//--------------------------------------------------------------
void testApp::update(){

	// update the sound playing system:
	ofSoundUpdate();	
	
	// (1) we increase px and py by adding vx and vy
	px += vx;
	py += vy;
	
	// (2) check for collision, and trigger sounds:
	// horizontal collisions:
	if (px < 0){
		px = 0;
		vx *= -1;
		dog.play();
	} else if (px > ofGetWidth()){
		px = ofGetWidth();
		vx *= -1;
		ow.play();
	}
	// vertical collisions:
	if (py < 0 ){
		py = 0;
		vy *= -1;
		rooster.play();
	} else if (py > ofGetHeight()){
		py = ofGetHeight();
		vy *= -1;
		beat.play();
	}
	// (3) slow down velocity:
	vx 	*= 0.996f;
	vy 	*= 0.996f;

	// (4) we use velocity for volume of the samples:
	float vel = sqrt(vx*vx + vy*vy);
	ow.setVolume(MIN(vel/5.0f, 1));
	beat.setVolume(MIN(vel/5.0f, 1));
	dog.setVolume(MIN(vel/5.0f, 1));
	rooster.setVolume(MIN(vel/5.0f, 1));

	// (5) grab the fft, and put in into a "smoothed" array,
	//		by taking maximums, as peaks and then smoothing downward
	float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
		
	}

}

//--------------------------------------------------------------
void testApp::draw(){   
    ofBackground(red, green, blue, 255);
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPopStyle();
	
	ofEnableAlphaBlending();
		ofSetColor(255,255,255,100);
		ofRect(500,ofGetHeight()-300,5*128,200);
	ofDisableAlphaBlending();
	
	// draw the fft resutls:
	ofSetColor(255,255,255,255);
	
	float width = (float)(5*128) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofRect(500+i*width,ofGetHeight()-100,width,-(fftSmoothed[i] * 200));
	}
	
	// finally draw the playing circle:

	ofEnableAlphaBlending();
		ofSetColor(255,255,255,20);
		ofCircle(px, py,50);	
	ofDisableAlphaBlending();
	
	ofSetHexColor(0xffffff);
	ofCircle(px, py,8);
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(kind == OFX_UI_WIDGET_BUTTON)
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        cout << name << "\t value: " << button->getValue() << endl;
    }
    else if(kind == OFX_UI_WIDGET_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << name << "\t value: " << toggle->getValue() << endl;
    }
    else if(kind == OFX_UI_WIDGET_IMAGEBUTTON)
    {
        ofxUIImageButton *button = (ofxUIImageButton *) e.widget;
        cout << name << "\t value: " << button->getValue() << endl;
    }
    else if(kind == OFX_UI_WIDGET_IMAGETOGGLE)
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.widget;
        cout << name << "\t value: " << toggle->getValue() << endl;
    }
	else if(kind == OFX_UI_WIDGET_LABELBUTTON)
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        cout << name << "\t value: " << button->getValue() << endl;
    }
    else if(kind == OFX_UI_WIDGET_LABELTOGGLE)
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        if(e.widget->getName() == "NEW YORK")
        {
            ofSetFullscreen(toggle->getValue());
        }
        else if(e.widget->getName() == "BOSTON")
        {
            
            ofSetFullscreen(toggle->getValue());
        }
        
    }
	else if(name == "B1")
	{
        ofxUIButton *button = (ofxUIButton *) e.widget;
        cout << "value: " << button->getValue() << endl;
	}
    else if(name == "B2")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        cout << "value: " << button->getValue() << endl;
    }
    else if(name == "T1")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value: " << toggle->getValue() << endl;
    }
    else if(name == "T2")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value: " << toggle->getValue() << endl;
    }


}
//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}
//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	if (key == 'f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	// add into vx and vy a small amount of the change in mouse:
	vx += (x - prevx) / 20.0f;
	vy += (y - prevy) / 20.0f;
	// store the previous mouse position:
	prevx = x;
	prevy = y;
}
 
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	prevx = x;
	prevy = y;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

