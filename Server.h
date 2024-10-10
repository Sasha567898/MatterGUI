#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <netinet/in.h>
#include <string>
#include <cstring>


#define SERVER_PORT 16666

struct DataFromSensors{
    u_int8_t bulb;
    int8_t temperature;
    u_int8_t humidity;
    bool motion;
    u_int16_t illuminance;
};
struct BulbInfo{
        bool buttonClicked = false;
        bool motionAcrivatedMode = false;
};


void deserialize(const char* buffer, DataFromSensors& packet);
void serialize(char* buffer, BulbInfo& packet);
int receiveData(int serverSocket, DataFromSensors &dataFromSensors);
int initializeServer();
int acceptClient();
void sendBulbInfoToClient(int clientSocket,  BulbInfo &data);
void cleanupServer();

#endif