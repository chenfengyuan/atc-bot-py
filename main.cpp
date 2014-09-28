#include <iostream>
#include "main.hpp"
#include "atc-bot/atc.hpp"
#include "atc-bot/atc-utils.hpp"
#include "atc-bot/atc_search.hpp"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <unordered_set>
struct cache{
    std::string rv;
    atc_search::search_result search_result;
};
void * make_cache(){
    return reinterpret_cast<void *>(new cache{});
}

void free_cache(void * p){
    cache * cache_ = reinterpret_cast<cache *>(p);
    delete cache_;
}

char const * search(void * cache_void, char * data){
    cache & cache_ = *(reinterpret_cast<cache *>(cache_void));
    std::ostringstream out;
    atc_utils::frame fm = atc_utils::read_status(data);
    std::vector<int> planes_has_path;
    std::unordered_set<int> planes_has_nopath;
    for(auto const & pair : fm.map.get_planes()){
        auto const & plane = pair.second;
        if(cache_.search_result[plane.get_no()][fm.clck].pos == plane.get_position())
            planes_has_path.push_back(plane.get_no());
        else{
            if(plane.get_altitude() > 0)
                planes_has_nopath.insert(plane.get_no());
            std::cout << pair.first << "\n";
            std::cout << cache_.search_result[plane.get_no()][fm.clck].pos << "\n";
            std::cout << plane.get_position() << ',' << plane.get_altitude() << "\n";
            cache_.search_result.erase(plane.get_no());
        }
    }
    for(int no : planes_has_path){
        fm.map.del_plane(no);
        for(auto const & pair: cache_.search_result[no]){
            atc_search::result_node const & node = pair.second;
            fm.map.mark_position(node.pos, node.clck, node.altitude);
        }
    }
    for(auto & pair : atc_search::search(fm)){
        planes_has_nopath.erase(pair.first);
        cache_.search_result[pair.first] = std::move(pair.second);
    }
    if(planes_has_nopath.size()){
        std::cout << "planes_has_no_path\n";
        for(int i : planes_has_nopath){
            std::cout << i << "\n";
        }
    }
    assert(planes_has_nopath.size() == 0);
    std::vector<int> planes_not_exists;
    for(auto & pair : cache_.search_result){
        char plane_no = static_cast<char>(pair.first - 0 + 'a');
        if(pair.second.end() == pair.second.find(fm.clck + 1)){
            planes_not_exists.push_back(pair.first);
            continue;
        }
        atc_search::result_node a = pair.second[fm.clck];
        atc_search::result_node b = pair.second[fm.clck + 1];
        if(a.altitude != b.altitude)
            out << plane_no << "a" << b.altitude  << "\n";
        if(!(a.pos.dir == b.pos.dir))
            out << plane_no << "t" << b.pos.dir.get_char() << "\n";
    }
    for(int i : planes_not_exists)
        cache_.search_result.erase(i);
    cache_.rv = out.str();
    return cache_.rv.c_str();
}
