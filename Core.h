#pragma once
#include <Connection.h>
#include <Event.h>

typedef struct connection_s connection_t;

struct listening_s{
    int fd;

    struct sockaddr_in *sockaddr;

    connection_t *connection;    
};

typedef struct listening_s listening_t;

struct cycle_s{
    connection_t *connection;
    listening_t *listening;
    event_t *read_event, *write_event;
};

typedef struct cycle_s cycle_t;


