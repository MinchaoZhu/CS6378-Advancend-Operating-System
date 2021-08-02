#ifndef _MYNET_CLIENT_H_
#define _MYNET_CLIENT_H_
#include <thread>

class Client{
public:
    Client();
    Client(unsigned long, int*); // neighbor id
    void start();
    void join();
    int* counter;
    unsigned long neighbor;

private:
    std::thread clientThread;
    void execute(Client* object, int* counter);

};


#endif