//
//  parser.cpp
//  example
//
//  Created by Ryo Yumoto on 2019/11/05.
//

#include "parser.hpp"
#include <Poco/InflatingStream.h>
#include "ofMain.h"

namespace ofx {
namespace als {
static const std::unordered_map<int, ofColor> kColorIndexMap{
    {0, ofColor(237,66,15)},
    {1, ofColor(193,94,0)},
    {2, ofColor(178,139,0)},
    {3, ofColor(131,150,7)},
    {4, ofColor(81,169,43)},
    {5, ofColor(2, 156, 142)},
    {6, ofColor(2 ,120,194)},
    {7, ofColor(0 ,0 ,254)},
    {8, ofColor{44, 80,166}},
    {9, ofColor(99,72,169)},
    {10, ofColor(123,123,123)},
    {11, ofColor(60, 60, 60)},
    {12, ofColor(253, 0, 0)},
    {13, ofColor(193, 187, 101)},
    {14, ofColor(167, 192, 3)},
    {15, ofColor(122, 199, 31)},
    {16, ofColor(51, 194, 1)},
    {17, ofColor(0, 192, 175)},
    {18, ofColor(0, 165, 241)},
    {19, ofColor(82, 124, 236)},
    {20, ofColor(135,104,233)},
    {21, ofColor(162,71,176)},
    {22, ofColor(191,59,109)},
    {23, ofColor(151,89,50)},
    {24, ofColor(250,108,1)},
    {25, ofColor(188,253,1)},
    {26, ofColor(129,255,91)},
    {27, ofColor(0,254,6)},
    {28, ofColor(2,253,164)},
    {29, ofColor(66,254,211)},
    {30, ofColor(2,233,255)},
    {31, ofColor(133,198,254)},
    {32, ofColor(145,166,255)},
    {33, ofColor(186,137,255)},
    {34, ofColor(218,102,229)},
    {35, ofColor(255,44,213)},
    {36, ofColor(253,168,7)},
    {37, ofColor(255,241,0)},
    {38, ofColor(227,244,5)},
    {39, ofColor(221,196,5)},
    {40, ofColor(188,157, 92)},
    {41, ofColor(137,181,122)},
    {42, ofColor(135,195,185)},
    {43, ofColor(156,178,199)},
    {44, ofColor(136,165,197)},
    {45, ofColor(198,139,125)},
    {46, ofColor(238,58,123)},
    {47, ofColor(252, 148,163)},
    {48, ofColor(254,163,108)},
    {49, ofColor(253,239,153)},
    {50, ofColor(210,229,148)},
    {51, ofColor(183,209,113)},
    {52, ofColor(168,168,168)},
    {53, ofColor(211,254,226)},
    {54, ofColor(203,241,251)},
    {55, ofColor(185,191,227)},
    {56, ofColor(205,187,229)},
    {57, ofColor(208,208,208)},
    {58, ofColor(225,231,231)},
    {59, ofColor(255,255,255)}
};
    
ofColor ConvertIndexToRgb(int color_index){
    auto itr = kColorIndexMap.find(color_index);
    if(itr != kColorIndexMap.end()){
        return (*itr).second;
    }else{
        return ofColor::white;
    }
}
    
    
bool Parser::open(const std::string& file_path){
    std::ifstream ifs;
    ifs.open(ofToDataPath(file_path).c_str());
    if(!ifs) return false;
    Poco::InflatingInputStream inflater(ifs, Poco::InflatingStreamBuf::STREAM_GZIP);
    if(!inflater) return false;
    auto result = als_xml_.load(inflater);
    if(result) {
        std::cout << "Description: " << result.description() << std::endl;
    }else{
        std::cerr << "Error description: " << result.description() << std::endl;
        return false;
    }
    return true;
}
    
    
std::string Parser::getVersion() const {
    return als_xml_.child("Ableton").attribute("Creator").value();
}


std::vector<std::string> Parser::getTrackNames() const {
    std::vector<std::string> ret;
    auto tracks_node = als_xml_.child("Ableton").child("LiveSet").child("Tracks").children();
    std::transform(tracks_node.begin(), tracks_node.end(), std::back_inserter(ret), [](pugi::xml_node node){
        return node.child("Name").child("EffectiveName").attribute("Value").as_string();
    });
    return ret;
}

    
std::vector<std::string> Parser::getAudioTrackNames() const {
    std::vector<std::string> ret;
    auto tracks_node = als_xml_.child("Ableton").child("LiveSet").child("Tracks").children("AudioTrack");
    std::transform(tracks_node.begin(), tracks_node.end(), std::back_inserter(ret), [](pugi::xml_node node){
        return node.child("Name").child("EffectiveName").attribute("Value").as_string();
    });
    return ret;
}
   
    
std::vector<std::string> Parser::getMidiTrackNames() const {
    std::vector<std::string> ret;
    auto tracks_node = als_xml_.child("Ableton").child("LiveSet").child("Tracks").children("MidiTrack");
    std::transform(tracks_node.begin(), tracks_node.end(), std::back_inserter(ret), [](pugi::xml_node node){
        return node.child("Name").child("EffectiveName").attribute("Value").as_string();
    });
    return ret;
}
    
    
std::vector<std::string> Parser::getReturnTrackNames() const {
    std::vector<std::string> ret;
    auto tracks_node = als_xml_.child("Ableton").child("LiveSet").child("Tracks").children("ReturnTrack");
    std::transform(tracks_node.begin(), tracks_node.end(), std::back_inserter(ret), [](pugi::xml_node node){
        return node.child("Name").child("EffectiveName").attribute("Value").as_string();
    });
    return ret;
}

std::vector<std::string>  Parser::getAudioAndMidiTrackNames() const {
    auto ret           = getTrackNames();
    auto ret_tracks    = getReturnTrackNames();
    auto result = std::remove_if(ret.begin(), ret.end(), [ret_tracks](std::string track){
        bool is_return_track = std::any_of(ret_tracks.begin(), ret_tracks.end(), [track](std::string ret_track){
            return track == ret_track;
        });
        return is_return_track;
    });
    ret.erase(result, ret.end());
    return ret;
}

    
std::vector<std::string> Parser::getSceneNames() const {
    std::vector<std::string> ret;
    auto scenes_node = als_xml_.child("Ableton").child("LiveSet").child("SceneNames");
    std::transform(scenes_node.begin(), scenes_node.end(), std::back_inserter(ret), [](pugi::xml_node node){
        return node.attribute("Value").as_string();
    });
    return ret;
}
    
std::vector<ClipSlot>  Parser::getClipSlotsTrackUnit(const std::string& track_name) const {
    std::vector<ClipSlot> ret;
    auto tracks_node = als_xml_.child("Ableton").child("LiveSet").child("Tracks").children();
    auto node_itr = std::find_if(tracks_node.begin(), tracks_node.end(), [track_name](pugi::xml_node node){
        return track_name == node.child("Name").child("EffectiveName").attribute("Value").as_string();
    });
    
    if (node_itr != tracks_node.end()){
        auto clip_slots_node = node_itr->child("DeviceChain").child("MainSequencer").child("ClipSlotList").children();
        std::string track_type = node_itr->name();
        auto clip_type =  (track_type == "AudioTrack") ? "AudioClip" : "MidiClip";
        std::transform(clip_slots_node.begin(), clip_slots_node.end(), std::back_inserter(ret), [clip_type](pugi::xml_node node){
            ClipSlot ret;
            auto  clip_node = node.child("ClipSlot").child("Value").child(clip_type);
            std::string name = clip_node.child("Name").attribute("Value").as_string();
            ofColor color = ConvertIndexToRgb(clip_node.child("ColorIndex").attribute("Value").as_int());
            bool has_clip = !name.empty() && color != ConvertIndexToRgb(0);
            if (has_clip) { // has clip
                ret.clip = std::make_shared<Clip>(name, color);
            }
            return ret;
        });
    }
    return ret;
}
    
std::vector<ClipSlot> Parser::getClipSlotsSceneUnit(const std::string& scene_name) const{
    std::vector<ClipSlot> ret;
    auto scenes_node = als_xml_.child("Ableton").child("LiveSet").child("SceneNames");
    auto node_itr = std::find_if(scenes_node.begin(), scenes_node.end(), [scene_name](pugi::xml_node node){
        return scene_name == node.attribute("Value").as_string();
    });
    if (node_itr != scenes_node.end()){
        auto scene_index = std::distance(scenes_node.begin(), node_itr);
        for(auto name : getAudioAndMidiTrackNames()){
            auto clips = getClipSlotsTrackUnit(name);
            ret.push_back(clips[scene_index]);
        }
    }
    return ret;
}


}
}

