#ifndef _MYNET_SERVER_H_
#define _MYNET_SERVER_H_

#include "util/config_parser.h"
#include "mynet/client_manager.h"

#include <vector>
#include <thread>

const int SERVERACTIVE = 0;
const int SERVERPASSIVE = 1;

class Server{
public:
    HostConfig cfg;
    vector<int> clientSocket; // client sockets
    ClientManager cmanager;
    fd_set readFds;
    int max_sd;
    int connectionsNum;
    Server();
    Server(int, int);
    std::thread serverThread;

    int status;

    void run();
    void execute();
    const char* newConnectionMessage;
    int UpdateConnectionsNum(int);

    unsigned long messageSent;

};


extern Server *server;

#endif