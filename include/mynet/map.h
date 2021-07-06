#ifndef _MYNET_MAP_H_
#define _MYNET_MAP_H_
#include <thread>


class Map{
public:
    Map();
    std::thread mapThread;

    int remainingMsg = 0;

    const char* AppMsg = "a";
    const char* CtlMsg = "c";
    void start();
    void execute();
    void sendAppMsg();

    int updateRemainingMsg(int);
};


#endif