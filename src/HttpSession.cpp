#include <iostream>
#include <HttpSession.h>

HttpSession::HttpSession(std::unique_ptr<HttpCodec> codec)
    : codec_(std::move(codec))
{
    codec_ -> setCallback(this);
}

void HttpSession::httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD){
    size_t len = 1024;
    char buf[len];
    ssize_t recved;

    connection_t *c;
    c = (connection_t*) ev->data;
    conn_ = c;

    if ((recved = read(c->fd, buf, len)) < 0){
        std::perror("Read from socket");
    }

    // begin processing http requests
    codec_ -> onIngress(buf, recved);
}

connection_t* HttpSession::getConn(){
    return conn_;
}

void HttpSession::onMessageBegin(HttpMessage* msg){

}

void HttpSession::onHeadersComplete(HttpMessage* msg){
    Handler* handler = controller_.getRequestHandler(msg);
    
    handler->onRequest(getConn(), msg);
    
    if (handler){
        handler->detach();
        handler = nullptr;
    }
}
