//
//  parser.hpp
//  example
//
//  Created by Ryo Yumoto on 2019/11/05.
//

#ifndef parser_hpp
#define parser_hpp

#include "pugixml.hpp"
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include "ofColor.h"

namespace ofx {
namespace als {
class Clip {
public:
    Clip(const std::string& name, const ofColor& color) : name_{name}, color_{color}{}
    const std::string& name()  const { return name_; }
    const ofColor&     color() const { return color_; }
private:
    std::string name_;
    ofColor     color_;
};
    
class ClipSlot{
public:
    ClipSlot();
    ClipSlot(const Clip& clip);
    operator bool() const { return clip_ ? true : false; }
    const std::shared_ptr<Clip> clip()  const { return clip_; }
private:
    std::shared_ptr<Clip> clip_;
};

class Track {
public:
    enum class TrackType { kAudio, kMidi, kReturn };
    Track(const std::string& name, TrackType type, const std::vector<ClipSlot> clip_slots);
    const std::vector<ClipSlot>& clip_slots() const { return clip_slots_;}
    const std::string& name() const { return name_; }
    bool  IsAudioTrack() const { return type_ == TrackType::kAudio ? true : false; }
    bool  IsMidiTrack()  const { return type_ == TrackType::kMidi ? true : false; }
    bool  IsReturnTrack() const { return type_ == TrackType::kReturn ? true : false; }
private:
    TrackType   type_;
    std::string name_;
    std::vector<ClipSlot> clip_slots_;
};

class LiveSet {
public:
    LiveSet(const std::vector<Track>& tracks);
    const std::vector<Track>& tracks() const { return tracks_; }
private:
    std::vector<Track> tracks_;
};

class Parser {
public:
    static LiveSet     GetLiveSet(const std::string& file_path);
    static std::string GetVersion(const std::string& file_path);
    
private:
    pugi::xml_document als_xml_;
};
} // namespace als
} // namespace ofx







#endif /* parser_hpp */
