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

extern Server *server;

Client::Client(){}

Client::Client(unsigned long _neighbor, int* _counter) {
    neighbor = _neighbor;
    counter = _counter;
}

void Client::start() {  
    clientThread = std::thread(&Client::execute, this, this, counter);
}

void Client::join() {
    clientThread.join();
}

void Client::execute(Client* object, int* counter) {
    std::cout << object->neighbor <<": client starts" << std::endl; 
    auto cfg = hostsConfig[object->neighbor];
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(sock > -1);

    struct sockaddr_in address;
    struct hostent * host;
    address.sin_family = AF_INET;
    address.sin_port = htons(cfg.port);
    // TODO  
    // host = gethostbyname(nodes[neighbor].hostName.c_str());
    // if (!host)
    // {
    //     std::cout << "error: unknown host\n";
    //     exit(1);
    // }
    // memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
    inet_pton(AF_INET, cfg.hostName.c_str(), &address.sin_addr);

    for(int i =0; i <10; i++) {
        if (connect(sock, (struct sockaddr *)&address, sizeof(address))) {
            sleep(3);
            if(i = 9) {
                close(sock);
                sock = -1;
            }
        }
        else break;
    }

    if(sock > -1) {
        for(int i = 0; i < server->clientSocket.size(); ++i) {
            if( server->clientSocket[i] == 0 )
            {
                server->clientSocket[i] = sock;
                FD_SET(sock, &server->readFds);
                std::cout << "Client, new connection added as fd index: " << i << std::endl; 
                send(sock, server->newConnectionMessage, strlen(server->newConnectionMessage), 0);
                break;
            }
        }
    }

}


