#include "mynet/map.h"
#include "mynet/server.h"
#include "util/config_parser.h"
#include "mynet/vector_clock.h"
#include "mynet/cl_snapshot.h"

#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <mutex>
#include <stdlib.h>
#include <time.h>


// MAP protocol


Map::Map() {
    srand(time(NULL));
}

void Map::start() {
    if(server->status == SERVERACTIVE) {
        int msgTobeSent = config.minPerActive;
        msgTobeSent += rand() % (config.maxPerActive - config.minPerActive);
        updateRemainingMsg(msgTobeSent);
    }
    mapThread = std::thread(&Map::execute, this);
    auto msgSender = std::thread(&Map::sendAppMsg, this);
    msgSender.detach();
}


void Map::execute() {
    char buffer[1024];
    
    while(1) {
        for(int i = 0; i < server->clientSocket.size(); ++i) {
            if( server->clientSocket[i] != 0 ) {  
                FD_SET(server->clientSocket[i], &server->readFds);
            }
        }

        if(server->UpdateConnectionsNum(0) >= server->clientSocket.size()) {
            // connection established
            select(server->max_sd + 1, &server->readFds, NULL, NULL, NULL);

            for(int i = 0; i < server->clientSocket.size(); ++i) {
                int fd = server->clientSocket[i];
                if(FD_ISSET(fd, &server->readFds)) {
                    int dataRead = recv(fd, buffer, sizeof(buffer)-1, 0);
                    if(dataRead <= 0) {
                        std::cout << "Connection closed, fd index: " << i << endl;
                        close(fd);
                        server->clientSocket[i] = 0;
                    }
                    else {
                        buffer[dataRead] = '\0';
                        int i = 0;
                        while(i < dataRead) {
                            if(buffer[i] == '{') {
                                if(buffer[i+1] == 'c') {
                                    int start = i+2;
                                    while(i < dataRead && buffer[i] != '}') {
                                        ++i;  
                                    }
                                    std::string data = "";
                                    while(start < i) {
                                        data = data + buffer[start];
                                        ++start;
                                    }

                                    int thisX = stoi(data);

                                    cout << "ctl: " << "marker " << thisX <<" reveived" << endl;
                                    if(thisX != clSnapshot->prevX) {
                                        clSnapshot->prevX = thisX;
                                        clSnapshot->takeSnapshot();
                                        clSnapshot->sendMarker(thisX);
                                    }


                                    i++;
                                }
                                else {
                                    // app msg
                                    // clock
                                    int start = i;
                                    while(i < dataRead && buffer[i] != '}') {
                                        ++i;  
                                    }
                                    std::string data = "";
                                    while(start <= i) {
                                        data = data + buffer[start];
                                        ++start;
                                    }
                                    auto v = vClock->toVector(data);
                                    vClock->updateClock(v);
                                    vClock->increClock();
                                    std::cout << "msg: " << data << std::endl;
                                
                                
                                    if(server->status == SERVERPASSIVE && server->messageSent < config.maxNumber) {
                                        server->status = SERVERACTIVE;
                                        int msgTobeSent = config.minPerActive;
                                        msgTobeSent += rand() % (config.maxPerActive - config.minPerActive);
                                        updateRemainingMsg(msgTobeSent);
                                    }
                                }
                            }
                            ++i;
                        }


                    }
                }
            }
        }
    }
}


void Map::sendAppMsg() {
    while(1) {
        if(server->UpdateConnectionsNum(0) >= server->clientSocket.size() && server->status == SERVERACTIVE) {
            while(updateRemainingMsg(0) > 0 && server->messageSent < config.maxNumber) {
                int i = rand() % server->clientSocket.size();
                const char* msg = vClock->toString().c_str();
                vClock->increClock();

                send(server->clientSocket[i], msg, strlen(msg), 0);
                server->messageSent += 1; 
                if(updateRemainingMsg(-1) == 0) {
                    server->status = SERVERPASSIVE;
                }
                usleep(config.minSendDelay * 1000);
            }
        }
    }
}


std::mutex mt;

int Map::updateRemainingMsg(int n) {
    mt.lock();
    remainingMsg += n;
    int ret = remainingMsg;
    mt.unlock();
    return ret;
}


