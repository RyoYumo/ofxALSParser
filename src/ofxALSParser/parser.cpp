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
namespace {
const std::unordered_map<int, ofColor> kColorIndexMap{
    {0, ofColor(255,147,166)},
    {1, ofColor(255,165,41)},
    {2, ofColor(203,153,38)},
    {3, ofColor(247,244,124)},
    {4, ofColor(191,251,0)},
    {5, ofColor(20, 255, 46)},
    {6, ofColor(36, 255,168)},
    {7, ofColor(92 ,255 ,232)},
    {8, ofColor{139, 196,255}},
    {9, ofColor(84,128,228)},
    {10, ofColor(145,167,255)},
    {11, ofColor(216, 108, 228)},
    {12, ofColor(230, 82, 159)},
    {13, ofColor(255, 255, 255)},
    {14, ofColor(255, 54, 54)},
    {15, ofColor(247, 108, 2)},
    {16, ofColor(153, 114, 74)},
    {17, ofColor(255, 240, 52)},
    {18, ofColor(134, 255, 102)},
    {19, ofColor(61, 195, 0)},
    {20, ofColor(0,191,175)},
    {21, ofColor(25,233,255)},
    {22, ofColor(13,164,238)},
    {23, ofColor(0,125,192)},
    {24, ofColor(136,108,228)},
    {25, ofColor(182,118,198)},
    {26, ofColor(255,57,212)},
    {27, ofColor(208,208,208)},
    {28, ofColor(226,103,89)},
    {29, ofColor(255,162,116)},
    {30, ofColor(221,173,113)},
    {31, ofColor(238,255,174)},
    {32, ofColor(211,227,152)},
    {33, ofColor(186,208,116)},
    {34, ofColor(155,196,141)},
    {35, ofColor(212,254,225)},
    {36, ofColor(205,241,248)},
    {37, ofColor(185,193,227)},
    {38, ofColor(205,187, 228)},
    {39, ofColor(174,152, 229)},
    {40, ofColor(228,220, 225)},
    {41, ofColor(169,169,169)},
    {42, ofColor(198,146,139)},
    {43, ofColor(183,129,86)},
    {44, ofColor(152,130,106)},
    {45, ofColor(191,186,104)},
    {46, ofColor(166,190,0)},
    {47, ofColor(125,176,77)},
    {48, ofColor(136,194,186)},
    {49, ofColor(154,179,196)},
    {50, ofColor(133,164,194)},
    {51, ofColor(131,147,204)},
    {52, ofColor(165,149,181)},
    {53, ofColor(191,159,190)},
    {54, ofColor(188,113,149)},
    {55, ofColor(123,123,123)},
    {56, ofColor(175,50,51)},
    {57, ofColor(169,81,48)},
    {58, ofColor(114,79,65)},
    {59, ofColor(219,194,0)},
    {60, ofColor(132,150,31)},
    {61, ofColor(83,159,49)},
    {62, ofColor(6,156,142)},
    {63, ofColor(34,99,132)},
    {64, ofColor(25,47,150)},
    {65, ofColor(47,82,162)},
    {66, ofColor(98,74,173)},
    {67, ofColor(163,74,173)},
    {68, ofColor(204,46,109)},
    {69, ofColor(60,60,60)}
};

ofColor convertColorIndexToRgb(int color_index){
    auto itr = kColorIndexMap.find(color_index);
    if(itr != kColorIndexMap.end()){
        return (*itr).second;
    }else{
        return ofColor::white;
    }
}
}
    
    
bool Parser::load(const std::string& file_path){
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
    
std::vector<ClipSlot>  Parser::getClipSlots(const std::size_t track_index) const {
    std::vector<ClipSlot> ret;
    auto tracks_node = als_xml_.child("Ableton").child("LiveSet").child("Tracks").children();
    auto node_itr = std::next(tracks_node.begin(), track_index);
    
    if (node_itr != tracks_node.end()){
        auto clip_slots_node = node_itr->child("DeviceChain").child("MainSequencer").child("ClipSlotList").children();
        std::string track_type = node_itr->name();
        auto clip_type =  (track_type == "AudioTrack") ? "AudioClip" : "MidiClip";
        std::transform(clip_slots_node.begin(), clip_slots_node.end(), std::back_inserter(ret), [clip_type](pugi::xml_node node){
            ClipSlot ret;
            auto  clip_node = node.child("ClipSlot").child("Value").child(clip_type);
            std::string name = clip_node.child("Name").attribute("Value").as_string();
            ofLog() << name;
            int color = clip_node.child("ColorIndex").attribute("Value").as_int();
            bool has_clip = !name.empty() || color != 0;
            if (has_clip) { // has clip
                ret.clip = std::make_shared<Clip>(name, convertColorIndexToRgb(color));
            }
            return ret;
        });
    }
    return ret;
}

}
}

