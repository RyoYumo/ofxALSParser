//
//  parser.hpp
//  example
//
//  Created by Ryo Yumoto on 2019/11/05.
//

#ifndef OFXALSPARSER_PARSER_HPP_
#define OFXALSPARSER_PARSER_HPP_

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
    enum class Type { kAudio, kMidi, kReturn };
    Track(const std::string& name, Type type, const std::vector<ClipSlot> clip_slots);
    const std::vector<ClipSlot>& clip_slots() const { return clip_slots_;}
    const std::string& name() const { return name_; }
    Type type() const { return type_; }
    bool  IsAudioTrack() const { return type_ == Type::kAudio ? true : false; }
    bool  IsMidiTrack()  const { return type_ == Type::kMidi ? true : false; }
    bool  IsReturnTrack() const { return type_ == Type::kReturn ? true : false; }
private:
    Type   type_;
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
};
} // namespace als
} // namespace ofx
#endif // OFXALSPARSER_PARSER_HPP_
