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

#define WORKER_NUMBER 8

#define MAX_EPOLLFD 1024

// if EPOLLEXCLUSIVE is on, do not need to enable accept_mutex
#define ACCEPT_MUTEX 0 // default off, because epoll can handle thunderding herd problem now

// the process get the lock will block until new connection comes in 
#define EPOLL_TIMEOUT 500

// the process did not get the lock will wait for 500 ms 
#define MUTEX_DELAY 500

// maximum free connectionseach process has 
#define MAX_CONNECTIONS 1024


