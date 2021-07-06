#include "mynet/vector_clock.h"
#include "util/config_parser.h"
#include "util/string_helper.h"
#include "mynet/server.h"

#include <mutex>
#include <string>
#include <iostream>

VectorClock::VectorClock() {
    clock = std::vector<int>(config.nodesNumber, 0);
}


std::mutex cmt; 

void VectorClock::updateClock(std::vector<int> c) {
    if(c.size() == clock.size()) {
        cmt.lock();
        for(int i = 0; i < c.size(); ++i) {
            clock[i] = max(clock[i], c[i]);
        }
        cmt.unlock();
    }
}

void VectorClock::increClock() {
        cmt.lock();
        clock[server->cfg.id] += 1; 
        cmt.unlock();
}

string VectorClock::toString() {
    cmt.lock();
    string ret = "{";
    for(int i = 0; i < clock.size(); ++i) {
        ret += to_string(clock[i]);
        ret += " ";
    }
    ret = ret.substr(0, ret.length() - 1);
    ret += "}";
    cmt.unlock();
    return ret;
}

std::vector<int> VectorClock::toVector(std::string s) {
    std::vector<int> ret = std::vector<int>(config.nodesNumber, 0);
    auto sv = split(s.substr(1, s.length() - 2), " ");
    if(sv.size() == ret.size()) {
        for(int i = 0; i < ret.size(); ++i) {
            ret[i] = stoi(sv[i]);
        }
    }
    return ret;
}


