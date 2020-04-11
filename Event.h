#pragma once
#include <functional>
#include <Core.h>
#include <conf.h>

struct event_s;
typedef struct event_s event_t;

using EventHandlerFunc = std::function<void(event_t* ev)>;
    
struct event_s{
    void *data;
    EventHandlerFunc handler;

    int active, accept;
};

// typedef struct event_s event_t;

