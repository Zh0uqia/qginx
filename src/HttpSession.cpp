#include <iostream>
#include <HttpSession.h>

HttpSession::HttpSession(std::unique_ptr<HttpCodec> codec)
    : codec_(std::move(codec))
{
    codec_ -> setCallback(this);
}

void HttpSession::httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD){
    size_t len = 80*1024;
    char buf[len];
    ssize_t recved;

    connection_t *c;
    c = (connection_t*) ev->data;

    recved = recv(c->fd, buf, len, 0);

    if (recved < 0) {
          /* Handle error. */
        
    }

    // begin processing http requests
    codec_ -> onIngress(buf, recved);
}


