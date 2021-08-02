#include <iostream>
#include "util/config_parser.h"
#include "util/test_interval.h"
#include "mynet/clock.h"
#include "mynet/server.h"
#include "mynet/request_queue.h"
#include "mynet/l_mutex.h"
#include <unistd.h>
#include <random>
#include <fstream>



using namespace std;

LClock* lClock;
Server* server;
LMutex* lMutex;

std::default_random_engine generator;


int writeLog(int start, int end) {
    std::ofstream outFile;
    outFile.open("out" +to_string(server->id) + ".txt", std::ios_base::app);
    string s = to_string(start) + " " + to_string(end);
    outFile << s << "\n";
    outFile.close();

}

int criticalFunc() {
	std::exponential_distribution<double> distribution2(1.0 * config.criticalExecTime);
    lMutex->cs_enter();
    auto start = lClock->increClock();
    double number = distribution2(generator);
    usleep(1000*(int)number);
    auto end = lClock->increClock();
    writeLog(start,end);
    lMutex->cs_leave();
}


int main(int argc, char * argv[]) {
    // ./Main {id} {conf_path}
    if(argc <= 1) {
        cout << "Too few arguments" << endl;
        return 1;
    }
    int id = stoi(argv[1]);
    char* path = argv[2];

    readConfig(path);
    if(id == -1) {
        auto t = new TestInterval();
        t->Test();
        return 0;
    }
    printConfig();
    lClock = new LClock();
    server = new Server(id);
    server->run();
    server->serverThread.join();
    server->cmanager.join();
    lMutex = new LMutex();
    
	std::exponential_distribution<double> distribution1(1.0 * config.innerRequestDelay);

    for(int i = 0; i < config.requestSum; ++i) {
        double number = distribution1(generator);
        usleep(1000*(int)number);
        criticalFunc();
    }
    cout << "test exit" << endl;
    while(1){}

    return 0;
}