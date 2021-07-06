#ifndef _MYNET_SERVER_H_
#define _MYNET_SERVER_H_

#include "util/config_parser.h"
#include "mynet/client_manager.h"

#include <vector>


const uint8_t SERVERACTIVE = 0;
const uint8_t SERVERPASSIVE = 1;

class Server{
public:
    HostConfig cfg;
    vector<int> clientSocket; // client sockets
    ClientManager cmanager;
    fd_set readFds;

    Server();
    Server(int);

    void Run(uint8_t);
    const char* newConnectionMessage;



private:
    unsigned long messageSent;
    uint8_t status;
};


extern Server *server;

#endif