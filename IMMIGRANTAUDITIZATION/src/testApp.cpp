#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 

    
    
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
	ofEnableSmoothing();
    
    font.loadFont("GUI/NewMedia Fett.ttf", 32);

	// load in sounds:
	sound[0].loadSound("sounds/6sounds/ed_high.wav");
	sound[1].loadSound("sounds/6sounds/ed_high_un.wav");
	sound[2].loadSound("sounds/6sounds/ed_mid.wav");
    sound[3].loadSound("sounds/6sounds/ed_mid_un.wav");
    sound[4].loadSound("sounds/6sounds/ed_low.wav");
    sound[5].loadSound("sounds/6sounds/ed_low_un.wav");
    for (int i =0; i<6; i++) {
        sound[i].setLoop(true);
    }
    
	rooster.loadSound("sounds/6sounds/background2.wav");
	rooster.setLoop(true);
    
	// load in city imgs
	img_baltimore.loadImage("imgs/baltimore.jpg");
    img_birmingham.loadImage("imgs/birmingham.jpg");
    img_boston.loadImage("imgs/boston.jpg");
    img_detroit.loadImage("imgs/detroit.jpg");
    img_indianapolis.loadImage("imgs/indianapolis.jpg");
    img_losangeles.loadImage("imgs/los-angeles.jpg");
    img_nashville.loadImage("imgs/nashville.jpg");
    img_washingtondc.loadImage("imgs/washington-dc.jpg");
    img_BG.loadImage("imgs/us-lights.jpg");
	
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
   
    
    red = 233; blue = 27; green = 52;
    float dim = 32;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
	
    drawPadding = false;
    gui = new ofxUICanvas(0,0,length+xInit, 400);
    gui->addWidgetDown(new ofxUILabel("IMMIGRANT-AUDITIZATION", OFX_UI_FONT_LARGE));
    gui->addSpacer(length, 2);
    //year
    vector<string> hnames; hnames.push_back("2000"); hnames.push_back("2005"); hnames.push_back("2010");
	gui->addWidgetDown(new ofxUIRadio(dim, dim, "Year", hnames, OFX_UI_ORIENTATION_HORIZONTAL));
    //city
    gui->addSpacer(length-xInit, 2);

    gui->addLabelButton("NEW YORK", false);
    gui->addLabelButton("BOSTON", false);
    gui->addLabelButton("DETROIT", false);
    gui->addLabelButton("BALTIMORE", false);
    gui->addLabelButton("BIRMINGHAM", false);
    gui->addLabelButton("INDIANAPOLIS", false);
    gui->addLabelButton("LOS ANGELES", false);
    gui->addLabelButton("NASHVILLE", false);
    gui->addLabelButton("WASHINGTON DC", false);

    //play buttons
//    gui->addSpacer(length-xInit, 2);
//    gui->addWidgetDown(new ofxUIImageButton(dim*2, dim*2, false, "GUI/pause.png", "IMAGE BUTTON"));
//    gui->addWidgetRight(new ofxUIImageToggle(dim*2, dim*2, false, "GUI/play.png", "IMAGE TOGGLE"));
    
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
	ofBackground(red, green, blue);
    

    // city img dimensions
    img_h = 120;
    img_w = 200;
    
    //p
    
    for (int i=0; i<6; i++) {
        
        float j = floor(i/2);
        
        rects[i].set(1440/2*(i%2), 900/3*(int(j)%3), 1440/2, 900/3);
    }

    // Matthew's Code
    city = "";
   
    //data
    boston[0] = 35.2; //ed-hi
    boston[1] = 2.5;//un-hi
    boston[2] = 37.6;//ed-mid
    boston[3] = 3.8;//un-mid
    boston[4]= 9.0;//ed - low
    boston[5]= 7.1;//un-low
    
    detroit[0] = 31.5; //ed-hi
    detroit[1] = 2.3;//un-hi
    detroit[2] = 38.2;//ed-mid
    detroit[3] = 4.5;//un-mid
    detroit[4]= 16.1;//ed - low
    detroit[5]= 9.9;//un-low
    
    cityQuality[0] = "High Education";
    cityQuality[1] = "High Ed Unemployed";
    cityQuality[2] = "Middle Education";
    cityQuality[3] = "Middle Ed Unemployed";
    cityQuality[4] = "Low Education";
    cityQuality[5] = "Low Ed Unemployed";
}


//--------------------------------------------------------------
void testApp::update(){

	// update the sound playing system:
	ofSoundUpdate();
	if (!rooster.getIsPlaying()) {
          rooster.play();
    }
	// (1) we increase px and py by adding vx and vy
	px += vx;
	py += vy;
	
	// (2) check for collision, and trigger sounds:
	// horizontal collisions:
	if (px < 0){
		px = 0;
		vx *= -1;
		
	} else if (px > ofGetWidth()){
		px = ofGetWidth();
		vx *= -1;
		
	}
	// vertical collisions:
	if (py < 0 ){
		py = 0;
		vy *= -1;
	} else if (py > ofGetHeight()){
		py = ofGetHeight();
		vy *= -1;
	}
	vx 	*= 0.996f;
	vy 	*= 0.996f;

    for (int i=0; i<6; i++) {
        if (rects[i].inside(px, py)) {
            
            for (int j=0; j<6; j++) {
                if (j!=i) {
                    sound[j].stop();
                }
            }
        
            if (!sound[i].getIsPlaying()) {
                sound[i].play();
               
            }
        }
    }
    
    if (city =="") {
        sound[0].setVolume(0);
        sound[1].setVolume(0);
        sound[2].setVolume(0);
        sound[3].setVolume(0);
        sound[4].setVolume(0);
        sound[5].setVolume(0);
    }
    
    if (city == "boston") {

        sound[0].setVolume(ofMap(boston[0], 13.1, 50.4, 0, 1,true));
        sound[1].setVolume(ofMap(boston[1], 13.1, 50.4, 0, 1,true));
        sound[2].setVolume(ofMap(boston[2],26.9,54.5,0,1,true));
        sound[3].setVolume(ofMap(boston[3],1.6,18.1,0,1,true));
        sound[4].setVolume(ofMap(boston[4],4.8,46.5,0,1,true));
        sound[5].setVolume(ofMap(boston[5],0.4,19.7,0,1,true));
        

   
    }
    
    if (city == "detroit") {
        
        sound[0].setVolume(ofMap(detroit[0],13.1,50.4,0,1,true)); //ed-hi
        sound[1].setVolume(ofMap(detroit[1],0,9.1,0,1,true)); //un-hi
        sound[2].setVolume(ofMap(detroit[2],26.9,54.5,0,1,true)); //ed-mid
        sound[3].setVolume(ofMap(detroit[3],1.6,18.1,0,1,true)); //un-mid
        sound[4].setVolume(ofMap(detroit[4],4.8,46.5,0,1,true));//ed-low
        sound[5].setVolume(ofMap(detroit[5],0.4,19.7,0,1,true));//un-low
        

    }
    cout<<city<<endl;

    cout<<sound[4].getVolume()<<endl;

    
	rooster.setVolume(0);

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
    
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPopStyle();
    
    ofSetColor(255);
    img_BG.draw(0,0,1440,900);
    
    
	// draw the fft resutls:
	ofSetColor(255,255,255,100);
	float width = (float)(1440) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofRect(i*width,ofGetHeight(),width,-(fftSmoothed[i] * 200));
	}
	
	// finally draw the playing circle:
    ofSetColor(255,255,255,20);
    ofCircle(px, py,50);
	
	ofSetHexColor(0xffffff);
	ofCircle(px, py,8);
    
    
    // draw rectangle hotspots
    for (int i=0; i<6; i++) {
        if (rects[i].inside(px, py)) {
            ofSetColor(0, 100);
            ofRect(rects[i]);
        }else{
            ofSetColor(255, 10);
            ofRect(rects[i]);
        }
    }

    
    if (city == "boston") {
        img_boston.draw(1100,375,img_w,img_h);
        for (int i=0; i<6; i++) {
            
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(boston[i])+"% "+ cityQuality[i], 0, 0);
        
                font.drawString(ofToString(boston[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2 -rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+ rect.getHeight()/2);
            }
        }
    
    }
    
    if (city == "baltimore") {
        img_baltimore.draw(50,50);
    }
    
    if (city == "birmingham") {
        img_birmingham.draw(50,50);
    }
    
    if (city == "detroit") {
        img_detroit.draw(770,320,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(boston[i])+"% "+ cityQuality[i], 0, 0);
                

                font.drawString(ofToString(detroit[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
                
            }
        }
    }
    
    if (city == "indianapolis") {
        img_indianapolis.draw(50,50);
    }
    
    if (city == "losangeles") {
        img_losangeles.draw(50,50);
    }
    
    if (city == "nashville") {
        img_nashville.draw(50,50);
    }
    
    if (city == "washingtondc") {
        img_washingtondc.draw(50,50);
    }
    
    
    
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

        if(name == "DETROIT"){
            if (button->getValue()) {
                city = "detroit";
                cout<<"ok"<<endl;
            }
            
        }
        if(name == "BOSTON"){
            if (button->getValue()) {
                city = "boston";
                cout<<"ok"<<endl;
            }
            
        }
        
    }
    else if(kind == OFX_UI_WIDGET_LABELTOGGLE)
    {
 
            
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

