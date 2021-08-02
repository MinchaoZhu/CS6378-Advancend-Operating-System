#include "util/config_parser.h"
#include "util/string_helper.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Config config;
vector<HostConfig> hostsConfig;

void readConfig(char* path) {
    string line;
    
    string conf_path = string(path);
    ifstream ifs(conf_path);

    std::getline(ifs, line);
    auto confNums = split(line, " ");

    config = {
        stoul(confNums[0]), stoul(confNums[1]), stoul(confNums[2]), stoul(confNums[3]),
    };

    for (unsigned long i = 0; std::getline(ifs, line); ) 
    {
        if(line.length() == 0 || (line.length() > 0 && line[0] == '#')) {
            continue;
        }
        auto tmp = split(line, " ");
        HostConfig newHostConfig = {
            stoul(tmp[0]), tmp[1], stoi(tmp[2])
        };
        if(newHostConfig.hostName != "localhost") {
            newHostConfig.hostName += ".utdallas.edu";
        }

        hostsConfig.push_back(newHostConfig);
    }
    
    ifs.close();

}


void printConfig() {
    std::cout << "nodeNumbers: " << config.nodesNumber << std::endl;
    std::cout << "interRequestDelay: " << config.innerRequestDelay << std::endl;
    std::cout << "criticalExecTime: " << config.criticalExecTime << std::endl;
    std::cout << "requestSum: " << config.requestSum << std::endl;
}