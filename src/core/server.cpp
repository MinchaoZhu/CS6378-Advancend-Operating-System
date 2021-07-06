#include "mynet/server.h"
#include "mynet/client_manager.h"

#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>



Server::Server(){}

// node id
Server::Server(int thisNode) {
    cfg = hostsConfig[thisNode];
    clientSocket = vector<int>(cfg.neighbors.size(), 0);
    std::string msg = "New connection established from node " + to_string(cfg.id); 
    newConnectionMessage = msg.c_str();
    std::cout << "connection msg is" << newConnectionMessage << std::endl;
    messageSent = 0;

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

// init state: SERVERACTIVE / SERVERPASSIVE
void Server::Run(uint8_t initState) {
    status = initState;
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
    FD_SET(listenfd, &readFds);

    struct sockaddr_in client_address;

    int max_sd = listenfd;
    int newSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    while(1) {
        memset(buffer, '\0', sizeof(buffer));
        select(max_sd + 1, &readFds, NULL, NULL, NULL);
        // accept new connection
        if(FD_ISSET(listenfd, &readFds)) {
            newSocket = accept(listenfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
            assert(newSocket > -1);
            max_sd = max(max_sd, newSocket);
            std::cout << newConnectionMessage << std::endl;
            send(newSocket, newConnectionMessage, strlen(newConnectionMessage), 0);
        }

        for(int i = 0; i < clientSocket.size(); ++i) {
            if( clientSocket[i] == 0 )
            {
                clientSocket[i] = newSocket;
                std::cout << "Server, new connection added as fd index: " << i << std::endl; 
                break;
            }
        }

        // check incomming socket
        for(int i = 0; i < clientSocket.size(); ++i) {
            int fd = clientSocket[i];
            if(FD_ISSET(fd, &readFds)) {
                int dataRead = recv(fd, buffer, sizeof(buffer-1), 0);
                if(ret <= 0) {
                    std::cout << "Connection closed, fd index: " << i << endl;
                    close(fd);
                    clientSocket[i] = 0;
                }
                else {
                    // some message
                    if(messageSent < config.maxNumber && status != SERVERACTIVE) {
                        status = SERVERACTIVE;
                        cmanager.newActiveInterval(); // to be active
                    }
                    buffer[dataRead] = '\0';
                    std::cout << buffer << std::endl;
                }
            }
        }
    }


}