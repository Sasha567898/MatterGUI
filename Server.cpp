
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <iostream> 
#include <ctime> 
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "MainFrame.h"
#include "App.h"

int serverSocket;

int initializeServer()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) 
    {
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
    {
        return -1;
    }
    if (listen(serverSocket, 1) == -1) 
    {
        return -1;
    }

    return 0;
}

int acceptClient() {  
    struct sockaddr_in clientAddr;
   
    socklen_t addrLen = sizeof(clientAddr);
     
    int  clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
     
    if (clientSocket == -1) 
    {
        return -1;
    }
    return clientSocket;
}


void deserialize(const char* buffer, DataFromSensors& packet) {
    memcpy(&packet, buffer, sizeof(DataFromSensors));
}
void serialize(char* buffer, BulbInfo& packet)
{
    memcpy(buffer, &packet, sizeof(BulbInfo));
}

int receiveData(int serverSocket,  DataFromSensors &dataFromSensors) {
    char buffer[sizeof(DataFromSensors)];
    
    ssize_t bytesReceived = recv(serverSocket, buffer, sizeof(DataFromSensors), 0);
    if (bytesReceived <= 0) 
    {
        return -1;
    }

    deserialize(buffer, dataFromSensors);
    return 0;
}
void sendBulbInfoToClient(int clientSocket,  BulbInfo &data)
{
   char buffer[sizeof(BulbInfo)];
   serialize(buffer, data);
   send(clientSocket, buffer, sizeof(BulbInfo), 0);
    
}
void cleanupServer() {
  
    close(serverSocket);
}