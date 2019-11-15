#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    auto result = ofSystemLoadDialog();
    if(result.bSuccess){
        bool b = parser.open(result.getPath());
        if(b) std::cout << parser.getVersion() << std::endl;
    }
    
    
    std::cout << "Ableton Version: " << parser.getVersion() << std::endl;
    auto track_names            = parser.getTrackNames();
    auto audio_track_names      = parser.getAudioTrackNames();
    auto midi_track_names       = parser.getMidiTrackNames();
    auto audio_midi_track_names = parser.getAudioAndMidiTrackNames();
    auto scene_names            = parser.getSceneNames();
    
    std::cout << "----Track List----\n";
    std::copy(track_names.begin(), track_names.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    
    std::cout << "----Scene List-----\n";
    std::copy(scene_names.begin(), scene_names.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    
    
    auto first_track_name = track_names[0];
    auto clip_slots = parser.getClipSlotsTrackUnit(first_track_name);
    
    std::for_each(clip_slots.begin(), clip_slots.end(), [](ofxALS::ClipSlot& slot){
        if(slot.clip == nullptr){
            std::cout << "No Clip" << std::endl;
        }else{
            std::cout << "Clip Name: " << slot.clip->name;
            std::cout << "Clip Color: " << slot.clip->color;
            std::cout << "\n";
        }
    });
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
}
