#ifndef _MYNET_CLIENT_MANAGER_H_
#define _MYNET_CLIENT_MANAGER_H_

#include "mynet/client.h"

#include <vector>


class ClientManager{
public:
    ClientManager();
    ClientManager(int);

private:
    int* counters;
    int sum;
    std::vector<Client> clients;

};













#endif