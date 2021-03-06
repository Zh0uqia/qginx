#pragma once
#include <conf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <time.h>
#include <iostream>
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
#include <signal.h>

#include <Core.h>
#include <MasterProcess.h>
// #include <archived/Dispatcher.h>
// #include <archived/RequestHandler.h>

class Server
{
public:
    ~ Server();
    void start(int p);
    void serverInit();
    void handNewConn();
    static void handleSigpipe(int signum);
    int setNonBlocking();

private:
    int serverFD;
    int port_;
    bool started_;
    
    char readBuffer_[BUFFERLENGTH];
    int newSocket_;
    struct sockaddr_in address_;
    
    cycle_t* cycle;
    listening_t* ls;
};

