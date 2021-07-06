#include <iostream>
#include "util/config_parser.h"
#include "mynet/server.h"
#include "mynet/map.h"
#include "mynet/vector_clock.h"
#include <thread>

using namespace std;

Server *server;
VectorClock* vClock;

int main(int argc, char * argv[]) {
    // ./Main {id} {is_active}
    readConfig();
    printConfig();
    if(argc <= 2) {
        cout << "Too few arguments" << endl;
        return 1;
    }
    int id = stoi(argv[1]);
    int isActive = stoi(argv[2]);
    if(isActive != SERVERACTIVE && isActive != SERVERPASSIVE) {
        cout << "Arguments falut" << endl;
        return 1;
    }

    HostConfig conf = hostsConfig[id];
    server = new Server(id, isActive);
    auto map = new Map();
    vClock = new VectorClock();
    server->run();
    map->start();
    map->mapThread.join();
    server->serverThread.join();

    
    return 0;
}