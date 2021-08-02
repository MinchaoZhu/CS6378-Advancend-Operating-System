#ifndef _MYNET_REQUEST_QUEUE_H_
#define _MYNET_REQUEST_QUEUE_H_
#include <queue>
using namespace std;

struct Request {
    int id; // node id
    int timestamp; // timestamp
};



class RequestQueue {
public:
    RequestQueue();
    void Push(int, int);
    void Push(Request);
    Request Pop();
    Request Top();
    void Remove(int);
    int Size();
    bool Empty();

private:
    priority_queue<Request> q;
    
};


#endif