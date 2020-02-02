//
//  utility.hpp
//  oFAbletonOscTest
//
//  Created by Ryo Yumoto on 2020/02/02.
//

#ifndef utility_hpp
#define utility_hpp

#include "parser.hpp"
#include <algorithm>

namespace ofx{
namespace als{
template<typename Predicate>
const std::vector<Track> PickUpTrack(const LiveSet& live_set, Predicate&& pred){
    std::vector<Track> ret;
    auto tracks = live_set.tracks();
    std::copy_if(tracks.begin(), tracks.end(), std::back_inserter(ret), std::forward<Predicate>(pred));
    return ret;
}

inline static const std::vector<Track> PickUpTrack(const LiveSet& live_set, Track::Type type){
    auto pred = [type](const Track& track){
        return track.type() == type ? true : false;
    };
    return PickUpTrack(live_set, pred);
}

inline static const std::vector<Track> PickUpAudioTrack(const LiveSet& live_set){
    return PickUpTrack(live_set, Track::Type::kAudio);
}

inline static const std::vector<Track> PickUpMidiTrack(const LiveSet& live_set){
    return PickUpTrack(live_set, Track::Type::kMidi);
}

inline static const std::vector<Track> PickUpReturnTrack(const LiveSet& live_set){
    return PickUpTrack(live_set, Track::Type::kReturn);
}

inline static const std::vector<Track> PickUpAudioMidiTrack(const LiveSet& live_set){
    auto pred = [](const Track& track){
        return track.type() != Track::Type::kReturn ? true : false;
    };
    return PickUpTrack(live_set, pred);
}

} // namespace als
} // namespace ofx

#endif /* utility_hpp */
