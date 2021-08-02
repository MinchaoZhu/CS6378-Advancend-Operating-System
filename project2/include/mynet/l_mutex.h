#ifndef _MYNET_L_MUTEX_H_
#define _MYNET_L_MUTEX_H_
#include "mynet/server.h"
#include "mynet/request_queue.h"
#include <vector>
#include <thread>
#include <unordered_map>

const int LOCKRELEASE = 0;
const int LOCKREQUEST = 1;
const int LOCKACQUIRE = 2;

const char REQUESTTYPE = 'a';
const char REPLYTYPE = 'b';
const char RELEASETYPE = 'c';


class LMutex{
public:
    LMutex();
    std::vector<int> inSockets; // connection sockets
    std::vector<int> outSockets; // connection sockets


    std::unordered_map<int,int> in;
    std::unordered_map<int,int> out;

    void cs_enter();
    void cs_leave();

    int status;
    RequestQueue rq;    
    std::thread lMutexThread;

    void execute();
    

private:
    int id;
    void request();
    void sendReply(int);
    void reply();
    void release();
    void run();

    string genMsg(char type, int id, int t);
    Request msgToRequest(string msg);

};

extern LMutex* lMutex;

#endif