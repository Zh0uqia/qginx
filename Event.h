#pragma once

typedef void (*event_handler_pt)();

struct event_s{
    void *data;
    event_handler_pt handler;
}

typedef struct event_s event_t;

/*
class Event
{
public:
    Event() {}
    ~Event() {}

private:

};
*/ 
