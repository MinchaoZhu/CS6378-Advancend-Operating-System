#include "mynet/cl_snapshot.h"
#include "mynet/vector_clock.h"
#include "util/config_parser.h"
#include "mynet/server.h"

#include <string.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

CLSnapshot::CLSnapshot() {}

CLSnapshot::CLSnapshot(char* path, int _n) {
    logPath = std::string(path);
    logPath = logPath.substr(0, logPath.length() - 4) + "-" + to_string(server->cfg.id) + ".out";
    n = _n;
    prevX = -1;
}

void CLSnapshot::start() {
    clsThread = std::thread(&CLSnapshot::execute, this);
    clsThread.detach();
}


void CLSnapshot::execute() {
    while(server->UpdateConnectionsNum(0) < server->clientSocket.size()) {
    }

    while(n > 0) {
        clSnapshot->prevX = n;
        usleep(config.snapshotDelay * 1000);
        sendMarker(n);
        --n;
    }
}

void CLSnapshot::sendMarker(int x) {
    for(int i = 0; i < server->clientSocket.size(); ++i) {
        std::string msgStr = "{c" + to_string(x) + "}";
        const char* msg = msgStr.c_str();
        if( server->clientSocket[i] != 0 ) {  
            send(server->clientSocket[i], msg, strlen(msg), 0);
        }
    }
}

void CLSnapshot::takeSnapshot() {
    std::ofstream outFile;
    outFile.open(logPath, std::ios_base::app);
    auto s = vClock->toString();
    s = s.substr(1, s.length() - 2);
    outFile << s << "\n";
    outFile.close();
}