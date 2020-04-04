#pragma once
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

enum EventState{
    ListenOK;
    ListenError;
    ReadOK;
    ReadError;
    WriteOK;
    WriteError;
};

class Epoll
{
public:
    int epollInit();

    EventState epollAddEvent();

    int epollDeleteEvent();

private:
    typedef union epoll_data{
        void *ptr;
        int fd;
    }epoll_data_t;

    struct epoll_event{
        uint32_t events;
        epoll_data_t data;
    };

    int epollFd_;
};

