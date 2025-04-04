#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class Line
{
public:
	ofVec3f start, end, velocity;

	float alpha; // Trasparenza della linea

	Line(ofVec3f s, ofVec3f e, ofVec3f v, float a)
		: start(s), end(e), velocity(v), alpha(a) {}
};

class ofApp : public ofBaseApp, public ofxMidiListener
{
public:
	void setup();
	void update();
	void draw();
	void newMidiMessage(ofxMidiMessage &msg);

private:
	ofEasyCam cam;							 // Camera 3D
	ofxMidiIn midiIn;						 // Input MIDI
	std::deque<ofxMidiMessage> midiMessages; // Coda dei messaggi MIDI
	std::vector<Line> lines;				 // Lista delle linee 3D
	int camPosition = 5000;
};
