#include "mynet/request_queue.h"

bool operator<(Request a, Request b) {
    if (a.timestamp ==  b.timestamp) {
        return a.id > b.id;
    }
    return a.timestamp > b.timestamp;
}

RequestQueue::RequestQueue() {
}

int RequestQueue::Size() {
    return q.size();
}

bool RequestQueue::Empty() {
    return q.empty();
}

// timestampe, id
void RequestQueue::Push(int i, int t) {
    q.push(Request{i,t});
}

// request
void RequestQueue::Push(Request r) {
    q.push(r);
}

Request RequestQueue::Pop() {
    if(!Empty()) {
        auto ret = q.top();
        q.pop();
        return ret;
    } 
    else {
        return Request{-1,-1};
    }
}

Request RequestQueue::Top() {
    return q.top();
}

void RequestQueue::Remove(int id) {
    vector<Request> v;
    while(!q.empty()) {
        auto top = q.top();
        q.pop();
        if(top.id != id) {
            v.push_back(top);
        } 
    }

    for(auto r : v) {
        q.push(r);
    }
}