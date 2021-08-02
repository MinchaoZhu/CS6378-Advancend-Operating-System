#include <mynet/client.h>
#include <mynet/server.h>

#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>

extern Server *server;

Client::Client(){}

Client::Client(unsigned long _neighbor, int* _counter) {
    neighbor = _neighbor;
    counter = _counter;
}

void Client::start() {  
    clientThread = std::thread(&Client::execute, this, this, counter);
}


void Client::execute(Client* object, int* counter) {
    std::cout << object->neighbor <<": client starts" << std::endl; 
    auto cfg = hostsConfig[object->neighbor];
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(sock > -1);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(cfg.port);

    struct hostent * host;
    host = gethostbyname(cfg.hostName.c_str());
    if (!host)
    {
        std::cout << "error: unknown host\n";
        exit(1);
    }

    address.sin_addr = *(struct in_addr *)host->h_addr_list[0];

    // inet_pton(AF_INET, host->h_addr_list[0], &address.sin_addr);
    // memcpy((char*)(&address),host->h_addr_list[0],host->h_length);

    for(int i =0; i < 10; i++) {
        if (connect(sock, (struct sockaddr *)&address, sizeof(address)) != 0) {
            sleep(3);
            if(i == 9) {
                close(sock);
                sock = -1;
            }
        }
        else break;
    }

    if(sock > -1) {
        server->max_sd = max(sock, server->max_sd);
        for(int i = 0; i < server->outSocket.size(); ++i) {
            if( server->outSocket[i] == 0 )
            {
                server->outSocket[i] = sock;                
                server->outSocketsId[i] = object->neighbor;

                server->UpdateOutConnectionsNum(1);

                FD_SET(sock, &server->readFds);
                std::cout << "New connection established to node " << to_string(cfg.id) << std::endl;
                std::string msg = "New connection established from node " + to_string(server->id); 
                auto newConnectionMessage = msg.c_str();
                send(sock, newConnectionMessage, strlen(newConnectionMessage), 0);

                break;
            }
        }
    }

}


void Client::join() {
    clientThread.join();
}