#include "mynet/l_mutex.h"
#include "mynet/server.h"
#include "mynet/clock.h"
#include "util/string_helper.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

extern Server *server;
extern LClock *lClock;

LMutex::LMutex(){
    id = server->id;
    inSockets = server->inSocket;
    outSockets = server->outSocket;
    for(int i = 0; i < server->outSocketsId.size(); ++i) {
        out[server->outSocketsId[i]] = server->outSocket[i];
    }
    rq.Push(Request{id, INT32_MAX});
    run();
}

void LMutex::run() {
    lMutexThread = std::thread(&LMutex::execute, this);
}

void LMutex::execute() {
    char buffer[2048];
    for(auto fd : outSockets) {
        auto msg = genMsg(REPLYTYPE, id, lClock->t);
        auto pMsg = msg.c_str();
        send(fd, pMsg, strlen(pMsg), 0);
    }
    lClock->increClock();
    while(1) {
        fd_set readFds;
        int max_sd = 0;
        for(int i = 0; i < inSockets.size(); ++i) {
            if( inSockets[i] != 0 ) { 
                max_sd = max(max_sd, inSockets[i]); 
                FD_SET(inSockets[i], &readFds);
            }
        }
        select(max_sd + 1, &readFds, NULL, NULL, NULL);
        
        // check incomming socket
        for(int i = 0; i < inSockets.size(); ++i) {
            int fd = inSockets[i];
            if(fd != 0 && FD_ISSET(fd, &readFds)) {
                int dataRead = recv(fd, buffer, sizeof(buffer)-1, 0);
                if(dataRead <= 0) {
                    std::cout << "Connection closed, fd index: " << i << endl;
                    close(fd);
                    inSockets[i] = 0;
                }
                else {
                    int j = 0;
                    while(j < dataRead) {
                        int left = j;
                        while(left < dataRead) {
                            if(buffer[left] == '{') {
                                break;
                            }
                            ++left;
                        }
                        int right = left + 1;
                        while(right < dataRead) {
                            if(buffer[right] == '}') {
                                break;
                            }
                            ++right;
                        }
                        if (right >= dataRead) {
                            break;
                        }
                        auto type = buffer[left+1];
                        auto r = msgToRequest(string(&buffer[left], right-left+1));

                        lClock->updateClock(r.timestamp);
                        if(type == REQUESTTYPE) {
                            rq.Push(r);
                            sendReply(out[r.id]);
                        }
                        else if(type == REPLYTYPE) {

                        }
                        else if(type == RELEASETYPE) {
                            rq.Remove(r.id);                           
                        } else {
                            ++j;
                            continue;
                        }

                        auto topRequest = rq.Top();
                        cout << "topRequest id: " << topRequest.id << endl;
                        cout << "topRequest timestamp: " << topRequest.timestamp << endl;
                        cout << "r timestamp: " <<  r.timestamp << endl;

                        if(status == LOCKREQUEST && topRequest.id == id && topRequest.timestamp < r.timestamp) {
                            status = LOCKACQUIRE;
                        }
                        // cout << type << " " << r.id << " " << r.timestamp << endl;
                        j = right + 1;
                    }           
                }
            }
        }
    }
}

void LMutex::cs_enter() {
    status = LOCKREQUEST;
    request();
    while(status == LOCKREQUEST) {
        // cout <<"status: " <<  status << endl;
        // cout << "rq top id: " << rq.Top().id  << endl;
    }
}

void LMutex::cs_leave() {
    status = LOCKRELEASE;
    rq.Remove(server->id);
    release();
}

void LMutex::request() {
    auto t = lClock->updateClock(1);
    rq.Push(id, t);
    for(auto fd : outSockets) {
        lClock->updateClock(1);
        auto msg = genMsg(REQUESTTYPE, id, t);
        auto pMsg = msg.c_str();
        send(fd, pMsg, strlen(pMsg), 0);
    }
}

void LMutex::release() {
    for(auto fd : outSockets) {
        auto t = lClock->updateClock(1);
        auto msg = genMsg(RELEASETYPE, id, t);
        auto pMsg = msg.c_str();
        send(fd, pMsg, strlen(pMsg), 0);
    }
}

void LMutex::reply() {
    for(auto fd : outSockets) {
        sendReply(fd);
    }
}

void LMutex::sendReply(int fd) {
    auto t = lClock->updateClock(1);
    auto msg = genMsg(REPLYTYPE, id, t);
    auto pMsg = msg.c_str();
    send(fd, pMsg, strlen(pMsg), 0);
}


string LMutex::genMsg(char type, int id, int t) {
    string msg = "{";
    msg.push_back(type);
    msg += to_string(id);
    msg.push_back(' ');
    msg += to_string(t);
    msg += "}";
    return msg;
}

Request LMutex::msgToRequest(string msg) {
    string data = msg.substr(2, msg.length() - 3);
    vector<string> v = split(data, " ");
    return Request{stoi(v[0]), stoi(v[1])};
}