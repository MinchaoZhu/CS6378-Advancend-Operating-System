#include "mynet/client_manager.h"
#include "util/config_parser.h"
#include <malloc.h>
#include <thread>

ClientManager::ClientManager(){}


// server id
ClientManager::ClientManager(int id) {
    int n = hostsConfig[id].neighbors.size();    
    clients = std::vector<Client>(n);
    counters = (int*)malloc(sizeof(int) * n);


    for(int i = 0; i < n; ++i) {
        counters[i] = 0;
        clients[i] = Client(hostsConfig[id].neighbors[i], &(counters[i]));
    }

    for(int i = 0; i < n; ++i) {
        clients[i].start();
    }
    

}





