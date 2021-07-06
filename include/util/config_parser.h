#ifndef _UTIL_CONFIG_PARSER_H_
#define _UTIL_CONFIG_PARSER_H_

#include <vector>
#include <string>
using namespace std;


struct Config {
    unsigned long nodesNumber;   // total number of nodes
    unsigned long minPerActive;  // min messages number can be sent once
    unsigned long maxPerActive;  // max messages number can be sent once
    unsigned long minSendDelay;  // time interval between two messages send
    unsigned long snapshotDelay; // time interval between two snapshot 
    unsigned long maxNumber;     // max messages number can be sent
};

struct HostConfig {
    unsigned long id;
    string hostName;
    int port;
    vector<unsigned long> neighbors;
};

// global config
extern Config config; // six config numbers
extern vector<HostConfig> hostsConfig; // config for each host
void readConfig();
void printConfig();


#endif