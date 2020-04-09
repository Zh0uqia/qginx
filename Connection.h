#pragma once
#include <Event.h>

struct connection_s{
    void *data;
    event_t *read;
    event_t *write;

    int fd;
};
