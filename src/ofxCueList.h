

#pragma once

#include <stdio.h>
#include <chrono>
#include "ofMain.h"

class ofxCueList : public ofThread{
public:

    void setup(const std::string &path);
    void addEvent(const std::string &eventName, const ofEvent<void> &event);
    
    void start();
    void stop();
    void rewind();
protected:
    
    void threadedFunction();
    int index;
    std::vector<std::pair<float, std::string>> cuelist;
    std::map<std::string, ofEvent<void>> events;
};

inline void ofxCueList::setup(const std::string &path){
    ofBuffer buffer = ofBufferFromFile(path);
    ofXml xml;
    xml.loadFromBuffer(buffer);
    xml.setTo("events");
    if(!xml.setTo("event")){
        ofLog(OF_LOG_FATAL_ERROR) << "event tag not found";
        OF_EXIT_APP(1);
    }
    
    do{
        float deltaTime = ofToFloat(xml.getAttribute("delta"));
        std::string name = xml.getAttribute("name");
        cuelist.emplace_back(deltaTime, name);
    }while(xml.setToSibling());
}

inline void ofxCueList::addEvent(const std::string &eventName, const ofEvent<void> &event){
    events.emplace(eventName, event);
}


inline void ofxCueList::threadedFunction(){
    
    for( auto cue : cuelist){
        float delta = cue.first;
        sleep(delta * 1000.0f);
        ofLog() << cue.second;
    }
    
}

inline void ofxCueList::start(){
    startThread(true);
}

inline void ofxCueList::stop(){
    stopThread();
}

inline void ofxCueList::rewind(){
    index = 0;

}