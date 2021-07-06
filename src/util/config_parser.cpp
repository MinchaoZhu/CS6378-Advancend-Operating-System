#include "util/config_parser.h"
#include "util/string_helper.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Config config;
vector<HostConfig> hostsConfig;

void readConfig() {
    string line;
    
    string conf_path = "./config/main.txt";
    ifstream ifs(conf_path);

    std::getline(ifs, line);
    auto confNums = split(line, " ");

    config = {
        stoul(confNums[0]), stoul(confNums[1]), stoul(confNums[2]), stoul(confNums[3]), stoul(confNums[4]), stoul(confNums[5])
    };

    for (unsigned long i = 0; std::getline(ifs, line); ) 
    {
        if(line.length() == 0 || (line.length() > 0 && line[0] == '#')) {
            continue;
        }

        if(hostsConfig.size() < config.nodesNumber) {
            auto tmp = split(line, " ");
            HostConfig newHostConfig = {
                stoul(tmp[0]), tmp[1], stoi(tmp[2])
            };
            if(newHostConfig.hostName != "localhost") {
                newHostConfig.hostName += ".utdallas.edu";
            }

            hostsConfig.push_back(newHostConfig);
        }
        else {
            auto tmp = split(line, " ");
            for(auto s : tmp) {
                if(s[0] == '#') {
                    break;
                }
                hostsConfig[i].neighbors.push_back(stoi(s));
            }
            if(++i == config.nodesNumber) {
                break;
            }
        }

    }
    
    ifs.close();

}


void printConfig() {
    std::cout << "nodeNumbers: " << config.nodesNumber << std::endl;
    std::cout << "minPerActive: " << config.minPerActive << std::endl;
    std::cout << "maxPerActive: " << config.maxPerActive << std::endl;
    std::cout << "minSendDelay: " << config.minSendDelay << std::endl;
    std::cout << "snapshotDelay: " << config.snapshotDelay << std::endl;
    std::cout << "maxNumber: " << config.maxNumber << std::endl;
}