#ifndef _MYNET_SERVER_H_
#define _MYNET_SERVER_H_

#include "util/config_parser.h"
#include "mynet/client_manager.h"

#include <vector>
#include <thread>


class Server{
public:
    int id;
    HostConfig cfg;
    vector<int> inSocket; // in sockets, will be destroyed
    vector<int> inSocketsId;
    vector<int> outSocket; // out sockets
    vector<int> outSocketsId;
    ClientManager cmanager;
    fd_set readFds;
    int max_sd;
    int inConnectionsNum;
    int outConnectionsNum;
    Server();
    Server(int);
    std::thread serverThread;

    int status;

    void run();
    void execute();
    const char* newConnectionMessage;
    int UpdateInConnectionsNum(int);
    int UpdateOutConnectionsNum(int);

    unsigned long messageSent;

};


extern Server *server;

#endif