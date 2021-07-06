#ifndef _MYNET_CL_SNAPSHOT_H_
#define _MYNET_CL_SNAPSHOT_H_

#include <string>
#include <thread>

class CLSnapshot{
public:
    CLSnapshot();
    CLSnapshot(char*, int);
    std::string logPath;
    
    std::thread clsThread;
    int n; // remaining snapshot time
    int prevX; // 0: no snapshot 1: snapshot taken

    void start();
    void execute();
    void takeSnapshot();
    void sendMarker(int);

};


extern CLSnapshot* clSnapshot;


#endif