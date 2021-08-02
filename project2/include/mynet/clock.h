#ifndef _MYNET_VECTOR_CLOCK_H_
#define _MYNET_VECTOR_CLOCK_H_

class LClock {
public:
    LClock();
    int t;
    int updateClock(int _t); //compare time with _t and return time
    int increClock(); // clock incremented by 1
};

extern LClock* lClock;

#endif