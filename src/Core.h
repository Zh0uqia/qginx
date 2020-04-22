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
    int total_connection; // maximum number of connections 

    int accept_disabled; // try to get mutex or not

    connection_t *free_connections; // free connection list 
    int free_connections_n; // number of free connection 

    connection_t *connection; // connection pool 
    event_t *read_event, *write_event; // read and write event pool 
    
    listening_t *listening; // listening socket 
};

// typedef struct cycle_s cycle_t;

struct connection_s{
    void *data;
    event_t *read;
    event_t *write;
    
    listening_t *listening;

    int fd;
};
