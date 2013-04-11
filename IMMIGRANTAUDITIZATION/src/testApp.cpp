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
    img_boise.loadImage("imgs/boise.jpg");
    img_boulder.loadImage("imgs/boulder.jpg");
    img_lakeland.loadImage("imgs/lakeland.jpg");
    img_newyork.loadImage("imgs/newyork.jpg");
    img_riverside.loadImage("imgs/riverside.jpg");
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
    float length = 260-xInit;
	
    drawPadding = false;
    gui = new ofxUICanvas(0,0,length+xInit, 400);
    gui->addWidgetDown(new ofxUILabel("IMMIGRANT-AUDITIZATION", OFX_UI_FONT_LARGE));
    gui->addSpacer(length, 2);
    //year
    vector<string> hnames; hnames.push_back("2000"); hnames.push_back("2005"); hnames.push_back("2010");
	gui->addWidgetDown(new ofxUIRadio(dim, dim, "Year", hnames, OFX_UI_ORIENTATION_HORIZONTAL));
    //city
    gui->addSpacer(length-xInit, 2);

    // gui->addLabelButton("NEW YORK", false);
    gui->addLabelButton("BOSTON", false);
    gui->addLabelButton("DETROIT", false);
    gui->addLabelButton("BALTIMORE", false);
//    gui->addLabelButton("BIRMINGHAM", false);
//    gui->addLabelButton("INDIANAPOLIS", false);
//    gui->addLabelButton("LOS ANGELES", false);
//    gui->addLabelButton("NASHVILLE", false);
//    gui->addLabelButton("WASHINGTON DC", false);

    gui->addLabelButton("BOISE", false);
    gui->addLabelButton("BOULDER", false);
    gui->addLabelButton("LAKELAND", false);
    gui->addLabelButton("NEW YORK", false);
    gui->addLabelButton("RIVERSIDE", false);

    
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
   
    //data: year 2000
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
    
    baltimore[0] = 42.9; //ed-hi
    baltimore[1] = 3.2;//un-hi
    baltimore[2] = 38.4;//ed-mid
    baltimore[3] = 4.0;//un-mid
    baltimore[4]= 17.4;//ed - low
    baltimore[5]= 5.4;//un-low

    boise[0] = 16.4; //ed-hi
    boise[1] = 2.3;//un-hi
    boise[2] = 39.1;//ed-mid
    boise[3] = 5.3;//un-mid
    boise[4]= 10.9;//ed - low
    boise[5]= 8.9;//un-low
    
    
    boulder[0] = 41.2; //ed-hi
    boulder[1] = 0.9;//un-hi
    boulder[2] = 27.5;//ed-mid
    boulder[3] = 1.6;//un-mid
    boulder[4]= 4.9;//ed - low
    boulder[5]= 3.9;//un-low
    
    
    lakeland[0] = 16.8; //ed-hi
    lakeland[1] = 3.4;//un-hi
    lakeland[2] = 37.5;//ed-mid
    lakeland[3] = 3.8;//un-mid
    lakeland[4]= 22.9;//ed - low
    lakeland[5]= 5.7;//un-low
    
    
    newyork[0] = 23.6; //ed-hi
    newyork[1] = 4.1;//un-hi
    newyork[2] = 41.6;//ed-mid
    newyork[3] = 7.3;//un-mid
    newyork[4]= 18.4;//ed - low
    newyork[5]= 10.6;//un-low
    
    
    riverside[0] = 13.1; //ed-hi
    riverside[1] = 3.3;//un-hi
    riverside[2] = 35.8;//ed-mid
    riverside[3] = 5.7;//un-mid
    riverside[4]= 16.1;//ed - low
    riverside[5]= 9.2;//un-low

    
    cityQuality[0] = "High Education";
    cityQuality[1] = "High Ed Unemployed";
    cityQuality[2] = "Middle Education";
    cityQuality[3] = "Middle Ed Unemployed";
    cityQuality[4] = "Low Education";
    cityQuality[5] = "Low Ed Unemployed";
    
    
    city_max[0] = 50.4;
    city_min[0] = 13.1;
    
    city_max[1] = 9.1;
    city_min[1] = 0;
    
    city_max[2] = 54.5;
    city_min[2] = 26.9;
    
    city_max[3] = 18.1;
    city_min[3] = 1.6;
    
    city_max[4] = 46.5;
    city_min[4] = 4.8;
    
    city_max[5] = 19.7;
    city_min[5] = 0.4;
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
	vx 	*= 0.999f;
	vy 	*= 0.999f;

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
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(boston[i], city_min[i], city_max[i], 0, 1,true));
        }
    }
    
    if (city == "detroit") {
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(detroit[i], city_min[i], city_max[i], 0, 1,true));
        }        
    }
    
    if (city == "boise") {
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(boise[i], city_min[i], city_max[i], 0, 1,true));
        }
    }
    
    if (city == "boulder") {
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(boulder[i], city_min[i], city_max[i], 0, 1,true));
        }
    }
    
    if (city == "lakeland") {
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(lakeland[i], city_min[i], city_max[i], 0, 1,true));
        }
    }
    
    if (city == "newyork") {
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(newyork[i], city_min[i], city_max[i], 0, 1,true));
        }
    }
    
    if (city == "riverside") {
        for (int i=0; i<6; i++) {
            sound[i].setVolume(ofMap(riverside[i], city_min[i], city_max[i], 0, 1,true));
        }
    }
    
    cout<<city<<endl;
    
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
            ofSetColor(255, 0);
            ofRect(rects[i]);
            ofSetColor(255, 255); // for imgs that follow
        }
    }

    
    if (city == "boston") {
        ofSetColor(255, 255);
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
        ofSetColor(255, 255);
        img_baltimore.draw(980,400,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(baltimore[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(baltimore[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
  
    }

    
    
    if (city == "birmingham") {
        img_birmingham.draw(800,500,img_w,img_h);
    }
    
    if (city == "detroit") {
        ofSetColor(255, 255);
        img_detroit.draw(770,320,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(detroit[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(detroit[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
           }
    
    if (city == "indianapolis") {
        img_indianapolis.draw(790,375,img_w,img_h);
    }
    
    if (city == "losangeles") {
        img_losangeles.draw(150,400,img_w,img_h);
    }
    
    if (city == "nashville") {
        img_nashville.draw(770,440,img_w,img_h);
    }
    
    if (city == "washingtondc") {
        img_washingtondc.draw(1000,400,img_w,img_h);
    }
    
    if (city == "boise") {
        ofSetColor(255, 255);
        img_boise.draw(200,250,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(boise[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(boise[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
    }
    
    if (city == "boulder") {
        ofSetColor(255, 255);
        img_boulder.draw(300,420,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(boulder[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(boulder[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
    }
    
    if (city == "lakeland") {
        ofSetColor(255, 255);
        img_lakeland.draw(900,550,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(lakeland[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(lakeland[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
    }
    
    if (city == "newyork") {
        ofSetColor(255, 255);
        img_newyork.draw(1050,350,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(newyork[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(newyork[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
    }
    
    if (city == "riverside") {
        ofSetColor(255, 255);
        img_riverside.draw(150,400,img_w,img_h);
        for (int i=0; i<6; i++) {
            if(rects[i].inside(px, py)){
                ofSetColor(255,200);
                ofRectangle rect = font.getStringBoundingBox(ofToString(riverside[i])+"% "+ cityQuality[i], 0, 0);
                font.drawString(ofToString(riverside[i])+"% "+ cityQuality[i],
                                rects[i].getPosition().x+rects[i].getWidth()/2-rect.getWidth()/2,
                                rects[i].getPosition().y+rects[i].getHeight()/2+rect.getHeight()/2);
            }
        }
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
                cout<<city<<endl;
            }
            
        }
        if(name == "BOSTON"){
            if (button->getValue()) {
                city = "boston";
                cout<<city<<endl;
            }
            
        }
        if(name == "BALTIMORE"){
            if(button->getValue()){
                city = "baltimore";
                cout<<city<<endl;
            }
        }
        if(name == "BIRMINGHAM"){
            if(button->getValue()){
                city = "birmingham";
                cout<<city<<endl;
            }
        }
        if(name == "INDIANAPOLIS"){
            if(button->getValue()){
                city = "indianapolis";
                cout<<city<<endl;
            }
        }
        if(name == "LOS ANGELES"){
            if(button->getValue()){
                city = "losangeles";
                cout<<city<<endl;
            }
        }
        if(name == "NASHVILLE"){
            if(button->getValue()){
                city = "nashville";
                cout<<city<<endl;
            }
        }
        if(name == "WASHINGTON DC"){
            if(button->getValue()){
                city = "washingtondc";
                cout<<city<<endl;
            }
        }
        
        if(name == "BOISE"){
            if(button->getValue()){
                city = "boise";
                cout<<city<<endl;
            }
        }

        if(name == "BOULDER"){
            if(button->getValue()){
                city = "boulder";
                cout<<city<<endl;
            }
        }

        if(name == "LAKELAND"){
            if(button->getValue()){
                city = "lakeland";
                cout<<city<<endl;
            }
        }
        
        if(name == "NEW YORK"){
            if(button->getValue()){
                city = "newyork";
                cout<<city<<endl;
            }
        }
        
        if(name == "RIVERSIDE"){
            if(button->getValue()){
                city = "riverside";
                cout<<city<<endl;
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

