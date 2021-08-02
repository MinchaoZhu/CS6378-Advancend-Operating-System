#ifndef _MYNET_VECTOR_CLOCK_H_
#define _MYNET_VECTOR_CLOCK_H_

#include <vector>
#include <string>

class VectorClock {
public:
    VectorClock();
    std::vector<int> clock;
    void updateClock(std::vector<int>);
    std::string toString();
    std::vector<int> toVector(std::string);
    void increClock();
};

extern VectorClock* vClock;

#endif