#pragma once

#include "ofMain.h"
#include "ofxUI.h"
class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
        void exit();
    
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofSoundPlayer 		rooster;
		ofSoundPlayer		sound[6];
        
		float 				* fftSmoothed;
		// we will draw a rectangle, bouncing off the wall:
		float 				px, py, vx, vy;

		int nBandsToGet;
		float prevx, prevy;
        
        //UI
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
        bool drawPadding;
        float red, green, blue;
        
        ofRectangle rects[6];
        ofRectangle rect;
    
        float boston[6];
        float detroit[6];
        float baltimore[6];
        float indianapolis[6];
        float losangeles[6];
        float nashville[6];
        float washingtondc[6];
        float birmingham[6];
        float boise[6];
        float boulder[6];
        float lakeland[6];
        float newyork[6];
        float riverside[6];

        string cityQuality[6];
        // city active
        string city;

        // city imgs
    
    int img_w;
    int img_h;
    
        ofImage img_baltimore;
        ofImage img_boston;
        ofImage img_detroit;
        ofImage img_indianapolis;
        ofImage img_losangeles;
        ofImage img_nashville;
        ofImage img_washingtondc;
        ofImage img_birmingham;
    ofImage img_boise;
    ofImage img_boulder;
    ofImage img_lakeland;
    ofImage img_newyork;
    ofImage img_riverside;
        ofImage img_BG;
    
        ofTrueTypeFont font;
    
};

