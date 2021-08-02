#include "mynet/clock.h"
#include <mutex>

LClock::LClock() {
    t = 0;
}


std::mutex cmt; 

int LClock::updateClock(int _t) {
    cmt.lock();
    t = t > _t ? t : _t;
    t += 1;
    cmt.unlock();
    return t;
}

int LClock::increClock() {
        cmt.lock();
        t++; 
        cmt.unlock();
        return t;
}
