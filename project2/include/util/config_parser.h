#ifndef _UTIL_CONFIG_PARSER_H_
#define _UTIL_CONFIG_PARSER_H_

#include <vector>
#include <string>
using namespace std;


struct Config {
    unsigned long nodesNumber;   // total number of nodes
    unsigned long innerRequestDelay;
    unsigned long criticalExecTime;
    unsigned long requestSum;
};

struct HostConfig {
    unsigned long id;
    string hostName;
    int port;
};

// global config
extern Config config; // four config numbers
extern vector<HostConfig> hostsConfig; // config for each host
void readConfig(char*);
void printConfig();


#endif