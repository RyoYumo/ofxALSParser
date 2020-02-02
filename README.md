# ofxALSParser

## Usage
```
ofxALS::LiveSet live_set;
void ofApp::setup(){
  auto result = ofSystemLoadDialog();
    
  // load .als
  if(result.bSuccess){
    std::cout << "Ableton Version : " << ofxALSParser::GetVersion(result.getPath()) << std::endl;
    live_set = ofxALSParser::GetLiveSet(result.getPath());
  }
    
  std::cout << "---All tracks list---\n";
  for(const auto& t : live_set.tracks()) std::cout << t.name() << std::endl;
    
  std::cout << "---Audio tracks list---\n";
  for(const auto& at : ofxALS::PickUpAudioTrack(live_set)) std::cout << at.name() << std::endl;
    
  std::cout << "---Midi tracks list----\n";
  for(const auto& mt : ofxALS::PickUpMidiTrack(live_set)) std::cout << mt.name() << std::endl;
    
  std::cout << "---Return tracks list---\n";
  for(const auto& rt : ofxALS::PickUpReturnTrack(live_set)) std::cout << rt.name() << std::endl;
    
    
  std::cout << "---Audio clips list----\n";
  for(const auto& at : ofxALS::PickUpAudioTrack(live_set)){
    for(const auto& slot : at.clip_slots()){
      if(slot){ // check empty
        std::cout << slot.clip()->name() << std::endl;
      }
    }
  }
}




```
