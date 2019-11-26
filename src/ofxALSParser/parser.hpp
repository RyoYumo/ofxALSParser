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
    Clip(const std::string& name, int color) : name{name}, color{color}{}
    std::string name;
    int color;
};
    
struct ClipSlot{
    std::shared_ptr<Clip> clip;
};

class Parser {
public:
    bool open(const std::string& file_path);
    std::string getVersion() const ;
    std::vector<std::string>  getTrackNames() const ;
    std::vector<std::string>  getAudioTrackNames() const;
    std::vector<std::string>  getMidiTrackNames() const;
    std::vector<std::string>  getReturnTrackNames() const;
    std::vector<std::string>  getAudioAndMidiTrackNames() const; // without return tracks
    std::vector<std::string>  getSceneNames() const;
    std::vector<ClipSlot>     getClipSlotsTrackUnit(const std::string& track_name) const;
    std::vector<ClipSlot>     getClipSlotsSceneUnit(const std::string& scene_name) const;
private:
    pugi::xml_document als_xml_;
};
    

namespace helper {
inline bool empty(const ClipSlot& slot){
    return slot.clip ? false : true;
}

ofColor convertColorIndexToRgb(int color_index);

}
    
} // namespace als
} // namespace ofx







#endif /* parser_hpp */
