#include <iostream>
#include "main.hpp"
#include "atc-bot/atc.hpp"
#include "atc-bot/atc-utils.hpp"
#include "atc-bot/atc_search.hpp"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <string>
struct cache{
    std::string rv;
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
    for(auto & pair : atc_search::search(fm)){
        char plane_no = static_cast<char>(pair.first - 0 + 'a');
        atc_search::result_node a = pair.second[fm.clck];
        atc_search::result_node b = pair.second[fm.clck + 1];
        if(a.altitude != b.altitude)
            out << plane_no << "a" << b.altitude  << "\n";
        if(!(a.pos.dir == b.pos.dir))
            out << plane_no << "t" << b.pos.dir.get_char() << "\n";
    }
    cache_.rv = out.str();
    return cache_.rv.c_str();
}
