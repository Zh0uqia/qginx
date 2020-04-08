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

    int epollAddListenEvent();

    int epollDeleteEvent();

private:
    int epollFd_;
};

