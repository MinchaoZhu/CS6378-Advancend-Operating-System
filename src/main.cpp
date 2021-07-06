#include <iostream>
#include "util/config_parser.h"
#include "mynet/server.h"
using namespace std;

Server *server;

int main(int argc, char * argv[]) {
    
    readConfig();
    printConfig();
    if(argc <= 1) {
        cout << "Too few arguments" << endl;
        return 1;
    }
    int id = stoi(argv[1]);

    HostConfig conf = hostsConfig[id];
    server = new Server(id);
    server->Run(SERVERACTIVE);
    
    return 0;
}