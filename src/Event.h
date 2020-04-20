#pragma once
#include <functional>
#include <Core.h>
#include <conf.h>

struct event_s;
typedef struct event_s event_t;
typedef struct cycle_s cycle_t;

typedef std::function<void(cycle_t* cycle, event_t* ev, int epollFD)> EventHandlerFunc;
    
struct event_s{
    void *data;
    EventHandlerFunc handl;
    
    int active;
    int accept;
};

// typedef struct event_s event_t;

