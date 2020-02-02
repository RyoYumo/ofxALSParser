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
struct Clip {
    Clip(const std::string& name, const ofColor& color) : name{name}, color{color}{}
    std::string name;
    ofColor color;
};
    
struct ClipSlot{
    std::shared_ptr<Clip> clip;
    operator bool() const { return clip ? true : false; }
};

class Parser {
public:
    bool load(const std::string& file_path);
    std::string getVersion() const ;
    std::vector<std::string>  getTrackNames() const ;
    std::vector<std::string>  getAudioTrackNames() const;
    std::vector<std::string>  getMidiTrackNames() const;
    std::vector<std::string>  getReturnTrackNames() const;
    std::vector<std::string>  getAudioAndMidiTrackNames() const; // without return tracks
    std::vector<std::string>  getSceneNames() const;
    std::vector<ClipSlot>     getClipSlots(const std::size_t track_index) const;
private:
    pugi::xml_document als_xml_;
};
} // namespace als
} // namespace ofx







#endif /* parser_hpp */
