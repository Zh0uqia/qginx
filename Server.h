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
#include <CommParser.h>
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

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    char buffer[1024] = {0};
    const char *hello = "Hello from server";

};

