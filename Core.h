#pragma once
#include <Event.h>

typedef struct connection_s connection_t;

struct listening_s{
    int fd;

    struct sockaddr_in *sockaddr;

    connection_t *connection;    
};

typedef struct listening_s listening_t;

struct cycle_s{
    connection_t *free_connections;
    int free_connections_n;

    connection_t *connection;
    event_t *read_event, *write_event;
    
    listening_t *listening;
};

typedef struct cycle_s cycle_t;

struct connection_s{
    void *data;
    event_t *read;
    event_t *write;
    
    listening_t *listening;

    int fd;
};
