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
Server::Server(int thisNode, int initState) {
    cfg = hostsConfig[thisNode];
    clientSocket = vector<int>(cfg.neighbors.size(), 0);
    std::string msg = "New connection established from node " + to_string(cfg.id); 
    connectionsNum = 0;
    newConnectionMessage = msg.c_str();
    messageSent = 0;
    status = initState;
    std::cout << "id: " << cfg.id << std::endl;
    std::cout << "hostName: " << cfg.hostName << std::endl;
    std::cout << "port: " << cfg.port << std::endl;
    std::cout << "neighbors: " << std::endl;
    for(size_t i = 0; i < cfg.neighbors.size(); ++i) {
        std::cout << " " << cfg.neighbors[i] ;
    }
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
    while(UpdateConnectionsNum(0) < clientSocket.size()) {
        memset(buffer, '\0', sizeof(buffer));
        FD_SET(listenfd, &readFds);
        for(int i = 0; i < clientSocket.size(); ++i) {
            if( clientSocket[i] != 0 ) {  
                FD_SET(clientSocket[i], &readFds);
            }
        }
        select(max_sd + 1, &readFds, NULL, NULL, NULL);
        if(UpdateConnectionsNum(0) == clientSocket.size()) {
            break;
        }
        // accept new connection
        if(FD_ISSET(listenfd, &readFds)) {
            newSocket = accept(listenfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
            assert(newSocket > -1);
            max_sd = max(max_sd, newSocket);
            std::string msg = "New connection established from node " + to_string(cfg.id); 

            newConnectionMessage = msg.c_str();
            send(newSocket, newConnectionMessage, strlen(newConnectionMessage), 0);
        

            for(int i = 0; i < clientSocket.size(); ++i) {
                if( clientSocket[i] == 0 )
                {
                    clientSocket[i] = newSocket;
                    UpdateConnectionsNum(1);
                    break;
                }
            }   
        }

        // check incomming socket
        for(int i = 0; i < clientSocket.size(); ++i) {
            int fd = clientSocket[i];
            if(FD_ISSET(fd, &readFds)) {
                int dataRead = recv(fd, buffer, sizeof(buffer)-1, 0);
                if(dataRead <= 0) {
                    std::cout << "Connection closed, fd index: " << i << endl;
                    close(fd);
                    clientSocket[i] = 0;
                }
                else {
                    buffer[dataRead] = '\0';
                    std::cout << "msg: " << buffer << std::endl;
                }
            }
        }

    }

    std::cout << "Connections Established: " << UpdateConnectionsNum(0) << " neighbors." << std::endl;


}

std::mutex cn_lock; //ConnectionsNum lock


int Server::UpdateConnectionsNum(int n) {
    cn_lock.lock();
    this->connectionsNum += n;
    int ret = this->connectionsNum;
    cn_lock.unlock();
    return ret;
}






