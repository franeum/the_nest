#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofBackground(255);
    ofEnableDepthTest();

    // Configura la telecamera
    cam.setPosition(0, 0, camPosition);
    cam.lookAt(ofVec3f(0, 0, 0));

    // Configura il MIDI
    midiIn.listInPorts(); // Stampa le porte MIDI disponibili
    midiIn.openPort(0);   // Apri la prima porta MIDI
    midiIn.addListener(this);
    midiIn.setVerbose(true);
}

//--------------------------------------------------------------
void ofApp::update()
{
    while (!midiMessages.empty())
    {
        ofxMidiMessage msg = midiMessages.front();
        midiMessages.pop_front();

        if (msg.status == MIDI_NOTE_ON && msg.velocity > 0)
        {
            int numLines = ofMap(msg.velocity, 0, 127, 1, 100);
            int _alpha = ofMap(msg.velocity, 0, 127, 50, 220);

            for (int i = 0; i < numLines; i++)
            {
                ofVec3f start, end;

                start.x = ofRandom(-ofGetWidth(), ofGetWidth() * 2);
                start.y = ofRandom(-ofGetHeight(), ofGetHeight() * 2);
                start.z = ofRandom(-500, 500);

                float angleXY = ofRandom(TWO_PI);
                float angleZ = ofRandom(-PI, PI);

                end.x = start.x + cos(angleXY) * ofGetWidth() * 1.5;
                end.y = start.y + sin(angleXY) * ofGetHeight() * 1.5;
                end.z = start.z + sin(angleZ) * 500;

                float speed = ofMap(msg.velocity, 0, 127, 0.5, 3.0);
                ofVec3f velocity(cos(angleXY) * speed, sin(angleXY) * speed, sin(angleZ) * speed);

                lines.emplace_back(start, end, velocity, (float)_alpha);
            }
        }
    }

    for (auto &line : lines)
    {
        line.start += line.velocity;
        line.end += line.velocity;
        line.alpha -= 0.5;                        // Diminuire la trasparenza gradualmente
        line.alpha = ofClamp(line.alpha, 0, 255); // Mantieni alpha tra 0 e 255
    }

    // Rimuove linee che sono scomparse
    lines.erase(std::remove_if(lines.begin(), lines.end(), [](const Line &l)
                               { return l.alpha <= 0 || abs(l.start.x) > ofGetWidth() * 2 ||
                                        abs(l.start.y) > ofGetHeight() * 2 ||
                                        abs(l.start.z) > 1000; }),
                lines.end());

    float time = ofGetElapsedTimef();

    // La camera gira intorno alla scena da una distanza maggiore
    float radius = 10000; // La distanza della camera dalla scena
    float speed = 0.05;   // Velocità di rotazione

    // Imposta la posizione della camera
    float camX = 0;                                  // radius * sin(time * speed); // Ruota intorno all'asse X
    float camY = radius * sin(time * (speed * 0.5)); // Ruota intorno all'asse Y
    float camZ = radius * sin(time * speed);         // Mantieni una distanza costante lungo l'asse Z

    cam.setPosition(camX, camY, camZ); // Imposta la posizione della camera
    cam.lookAt(ofVec3f(0, 0, 0));
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();

    for (auto &line : lines)
    {
        ofSetColor(0, 0, 0, line.alpha); // Bianco con trasparenza dinamica
        ofSetLineWidth(1);
        ofDrawLine(line.start, line.end);
    }

    cam.end();
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &msg)
{
    midiMessages.push_back(msg);
    if (midiMessages.size() > 10)
    { // Evita troppi messaggi in memoria
        midiMessages.pop_front();
    }
}
