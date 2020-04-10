#pragma once
#include <conf.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <Event.h>

enum EventState{
    ListenOK = 1,
    ListenError,
    ReadOK,
    ReadError,
    WriteOK,
    WriteError
};

class Epoll
{
public:
    int epollInit();

    int epollAddEvent(int);

    int epollDeleteEvent();

private:
    int epollFd_;
};

