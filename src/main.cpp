#include <iostream>
#include "util/config_parser.h"
using namespace std;


int main(int argc, char * argv[]) {
    readConfig();
    if(argc <= 1) {
        cout << "Too few arguments" << endl;
    }

    int id = stoi(argv[1]);

    HostConfig conf = hostsConfig[id];

    


}