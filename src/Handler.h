#pragma once
#include <conf.h>
#include <Core.h>
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
#include <fcntl.h>

#include <Response.h>
#include <Epoll.h>

class Handler
{
public:
    int setNonBlock(int fd);
    void acceptEventHandler(cycle_t *cycle, event_t* ev, int epollFD);
    connection_t* getConnection(cycle_t* cycle, int sFD);
    void httpInitConnection(connection_t *c, int epollFD);
 
private:
    Response rp;
    Epoll epl;
};
