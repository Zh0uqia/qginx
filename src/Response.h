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
    ssize_t readn(int fd, std::string &inBuffer, bool &isZero);
    ssize_t writen(int fd, std::string& buf, size_t n);
    void  httpWriteHandler(cycle_t *cycle, event_t *ev, int epollFD);
    void httpRequestAnalyze(std::string& inBuffer);
 
    void httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD);
    void freeConnection(cycle_t *cycle, connection_t *c, int epollFD);
 
private:
    Epoll epl;
    RequestHandler requestHandler;

    std::string inBuffer;
    std::string outBuffer_;
};

