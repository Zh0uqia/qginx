#pragma once
#include <functional>
#include <Core.h>
#include <conf.h>

struct event_s;
typedef struct event_s event_t;

typedef std::function<void(event_t* ev)> EventHandlerFunc;
    
struct event_s{
    void *data;
    EventHandlerFunc handl;

    int active;
    int accept;
};

// typedef struct event_s event_t;

