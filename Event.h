#pragma once
#include <functional>

// typedef void (*event_handler_pt)();
using eventHandlerPtr = std::function<void()>;
    

struct event_s{
    void *data;
    eventHandlerPtr handler;

    int active, accept;
};

typedef struct event_s event_t;

