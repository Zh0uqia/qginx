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

#include <RequestHandler.h>
#include <Dispatcher.h>

int serverFD;

class Server
{
public:
    void start(int p);
    int serverInit();
    void handNewConn();
    static void handleSigpipe(int signum);
    int setNonBlocking();

private:
    int port_;
    bool started_;
    
    char readBuffer_[BUFFERLENGTH];
    int newSocket_;
    struct sockaddr_in address_;

};

