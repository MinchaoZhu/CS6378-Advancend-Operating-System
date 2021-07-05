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
    
    string conf_path = "./config/main.conf";
    ifstream ifs(conf_path);

    std::getline(ifs, line);
    auto confNums = split(line, " ");

    config = {
        stoul(confNums[0]), stoul(confNums[1]), stoul(confNums[2]), stoul(confNums[3]), stoul(confNums[4]), stoul(confNums[5])
    };

    for (int i = 0; std::getline(ifs, line); ) 
    {
        if(line.length() == 0 || (line.length() > 0 && line[0] == '#')) {
            continue;
        }

        if(hostsConfig.size() < config.nodesNumber) {
            auto tmp = split(line, " ");
            HostConfig newHostConfig = {
                stoi(tmp[0]), tmp[1], tmp[2]
            };
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