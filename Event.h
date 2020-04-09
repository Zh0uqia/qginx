#pragma once

typedef void (*event_handler_pt)();

struct event_s{
    void *data;
    event_handler_pt handler;

    int active;
};

typedef struct event_s event_t;

