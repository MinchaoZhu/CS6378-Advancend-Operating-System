#ifndef _UTIL_CONFIG_PARSER_H_
#define _UTIL_CONFIG_PARSER_H_

#include <vector>
#include <string>
using namespace std;


struct Config {
    unsigned int nodesNumber;   // total number of nodes
    unsigned int minPerActive;  // min messages number can be sent once
    unsigned int maxPerActive;  // max messages number can be sent once
    unsigned int minSendDelay;  // time interval between two messages send
    unsigned int snapshotDelay; // time interval between two snapshot 
    unsigned int maxNumber;     // max messages number can be sent
};

struct HostConfig {
    int id;
    string hostName;
    string port;
    vector<int> neighbors;
};

// global config
extern Config config; // six config numbers
extern vector<HostConfig> hostsConfig; // config for each host
void readConfig();

#endif