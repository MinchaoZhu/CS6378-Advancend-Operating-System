#include <iostream>
#include "util/config_parser.h"
#include "mynet/server.h"
#include "mynet/map.h"
#include "mynet/vector_clock.h"
#include "mynet/cl_snapshot.h"
#include <thread>

using namespace std;

Server *server;
VectorClock* vClock;
CLSnapshot* clSnapshot;

int main(int argc, char * argv[]) {
    // ./Main {id} {is_active} {conf_path} {snap_shots_number}
    if(argc <= 4) {
        cout << "Too few arguments" << endl;
        return 1;
    }
    int id = stoi(argv[1]);
    int isActive = stoi(argv[2]);
    char* path = argv[3];
    int ssnumber = stoi(argv[4]);
    if(isActive != SERVERACTIVE && isActive != SERVERPASSIVE) {
        cout << "Arguments falut" << endl;
        return 1;
    }

    readConfig(path);
    printConfig();

    HostConfig conf = hostsConfig[id];
    server = new Server(id, isActive);
    auto map = new Map();
    vClock = new VectorClock();
    clSnapshot = new CLSnapshot(path, ssnumber);
    server->run();
    map->start();
    clSnapshot->start();
    map->mapThread.join();
    server->serverThread.join();

    
    return 0;
}