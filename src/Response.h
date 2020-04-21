#pragma once
#include <conf.h>
#include <stdlib.h>

#include <Core.h>
#include <RequestHandler.h>
#include <Dispatcher.h>
#include <Epoll.h>

class Response
{
public:
    void httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD);
    void freeConnection(cycle_t *cycle, connection_t *c, int epollFD);
 
private:
    Epoll epl;
};

