#include "mynet/client_manager.h"
#include "util/config_parser.h"
#include <malloc.h>
#include <thread>
#include <iostream>

ClientManager::ClientManager(){}


// server id
ClientManager::ClientManager(int id) {
    int n = config.nodesNumber;    
    clients = std::vector<Client>();
    counters = (int*)malloc(sizeof(int) * (n-1));


    for(int i = 0; i < n; ++i) {
        if(id == i) {
            continue;
        }
        counters[i] = 0;
        clients.push_back(Client(i, &(counters[i])));
    }

    for(int i = 0; i < n - 1; ++i) {
        clients[i].start();
    }

}

void ClientManager::join() {
    for(int i = 0; i < clients.size(); ++i) {
        clients[i].join();
    }
}







