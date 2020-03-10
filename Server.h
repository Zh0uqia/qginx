#pragma once
#include <conf.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#include <Server.h>
#include <RequestHandler.h>
#include <Dispatcher.h>

class Server
{
public:
    void start(int p);
    void close();
    void handNewConn();

private:
    int port_;
    bool started_;

    char readBuffer_[BUFFERLENGTH];
    int serverFD_, newSocket_;
    struct sockaddr_in address_;

};

