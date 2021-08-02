#include "mynet/server.h"
#include "mynet/client_manager.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>



Server::Server(){}

// node id, init state
Server::Server(int thisNode) {
    id = thisNode;
    cfg = hostsConfig[thisNode];
    std::string msg = "New connection established from node " + to_string(cfg.id); 
    inConnectionsNum = 0;
    outConnectionsNum = 0;
    newConnectionMessage = msg.c_str();
    messageSent = 0;
    inSocket = std::vector<int>(config.nodesNumber-1, 0);
    outSocket = std::vector<int>(config.nodesNumber-1, 0);
    inSocketsId = std::vector<int>(config.nodesNumber-1, 0);
    outSocketsId = std::vector<int>(config.nodesNumber-1, 0);
    std::cout << "id: " << id << std::endl;
    std::cout << "hostName: " << cfg.hostName << std::endl;
    std::cout << "port: " << cfg.port << std::endl;

    std::cout << std::endl;
    
    cmanager = ClientManager(thisNode);
    
}

void Server::run() {
    serverThread = std::thread(&Server::execute, server);
}

// init state: SERVERACTIVE / SERVERPASSIVE
void Server::execute() {
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(cfg.port);
    address.sin_addr.s_addr = INADDR_ANY;

    // create listen fd
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    
	int optval = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listenfd, 5);
    assert(ret != -1);

    char buffer[1024]; // data buffer
    
    FD_ZERO(&readFds);

    struct sockaddr_in client_address;

    max_sd = listenfd;
    int newSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    while(UpdateInConnectionsNum(0) < inSocket.size()) {
        memset(buffer, '\0', sizeof(buffer));
        FD_SET(listenfd, &readFds);
        for(int i = 0; i < inSocket.size(); ++i) {
            if( inSocket[i] != 0 ) {  
                FD_SET(inSocket[i], &readFds);
            }
        }

        select(max_sd + 1, &readFds, NULL, NULL, NULL);

        // accept new connection
        if(FD_ISSET(listenfd, &readFds)) {
            newSocket = accept(listenfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
            assert(newSocket > -1);
            max_sd = max(max_sd, newSocket);
            std::string msg = "New connection established from node " + to_string(cfg.id); 
            cout << msg << endl;
            newConnectionMessage = msg.c_str();
            send(newSocket, newConnectionMessage, strlen(newConnectionMessage), 0);
        

            for(int i = 0; i < inSocket.size(); ++i) {
                if( inSocket[i] == 0 )
                {
                    inSocket[i] = newSocket;
                    UpdateInConnectionsNum(1);
                    break;
                }
            }   
        }

    }

    // std::cout << UpdateInConnectionsNum(0) << " in channels established."<<std::endl;
    // for(auto fd : outSocket) {
    //     close(fd);
    //     std::cout << "connection closed " << fd << std::endl;
    // }
}

std::mutex cn1_lock; //ConnectionsNum lock
std::mutex cn2_lock; //ConnectionsNum lock


int Server::UpdateInConnectionsNum(int n) {
    cn1_lock.lock();
    this->inConnectionsNum += n;
    int ret = this->inConnectionsNum;
    cn1_lock.unlock();
    return ret;
}

int Server::UpdateOutConnectionsNum(int n) {
    cn2_lock.lock();
    this->outConnectionsNum += n;
    int ret = this->outConnectionsNum;
    cn2_lock.unlock();
    return ret;
}





