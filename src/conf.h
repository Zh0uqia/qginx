#pragma once

#include <iostream>
#include <sys/types.h>       // For data types
#include <errno.h>           // For errno 
#include <string>

#ifdef DEBUG
#define dbPrint(x) std::cout << x;
#else
#define dbPrint(x);
#endif
 
#define PORT 8080
#define BUFFERLENGTH 1024
#define WORKER_NUMBER 2
#define MAX_PROCESSES 16
#define MAX_EPOLLFD 1024
#define EPOLL_TIMEOUT 1000
#define MAX_CONNECTIONS 20000
