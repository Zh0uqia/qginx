#include <iostream>
#include <Epoll.h>

int Epoll::epollInit(){
    epollFd_ = epoll_create1(0);

    if (epollFd_ < 0) {
        perror("Epoll create failed");
        return -1;
    }

    return epollFd_;
}

Epoll::~Epoll(){
    if (epollFd_ >= 0)
        close(epollFd_);
}

// add listening socket to epoll 
// level triggered
int Epoll::epollAddEvent(int ep, event_t *ev, intptr_t event, uintptr_t flags){
    int op;
    uint32_t events, prev;
    event_t *e;
    connection_t *c;
    struct epoll_event ee;

    c = (connection_t*) ev->data;
    events = (uintptr_t) event;

    if (event == READ_EVENT) {
        e = c->write;
        prev = EPOLLOUT;

    }else{
        e = c->read;
        prev = EPOLLIN | EPOLLRDHUP;
    }

    if (e->active == 1) {
        op = EPOLL_CTL_MOD;
        events |= prev;
    } else {
        op = EPOLL_CTL_ADD; // accept event 
    }
    
    if (flags & EPOLLEXCLUSIVE)
        events &= ~EPOLLRDHUP; // disable EPOLLRDHUP

    ee.events = events | (uint32_t) flags; // set epoll flags here 
    ee.data.ptr = c;
    // ee.data.fd = c->fd; // fatal error! epoll_data is a union, not a struct 

    if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
        dbPrint("ev.events is " << ee.events \
                << " and flag is " << flags << std::endl);
        return 0;
    }
    
    ev->active = 1;

    return 1;

}

// delete listening socket from epoll 
int Epoll::epollDeleteEvent(int ep, event_t *ev, intptr_t event, uintptr_t flags){
    int op;
    uint32_t prev;
    event_t *e;
    connection_t *c;
    struct epoll_event ee;

    c = (connection_t*) ev->data;

    if (event == READ_EVENT) {
        e = c->write;
        prev = EPOLLOUT;

    } else {
        e = c->read;
        prev = EPOLLIN | EPOLLRDHUP;
    }

    if (e->active == 1) {
        op = EPOLL_CTL_MOD;
        ee.events = prev | (uint32_t) flags;
        ee.data.ptr = c;

    } else {
        op = EPOLL_CTL_DEL;
        ee.events = 0;
        ee.data.ptr = NULL;
    }

    if (epoll_ctl(ep, op, c->fd, &ee) == -1){
        return 0;
    }
    

    ev->active = 0;
        
    return 1;
}

// add connection (socket of accept()) to epoll
// edge triggered
int Epoll::epollAddConn(int ep, event_t *ev){
    connection_t *c;
    struct epoll_event ee;
    
    c = (connection_t*) ev->data;

    ee.data.ptr = c;
    ee.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP; // edge triggered
    
    if (epoll_ctl(ep, EPOLL_CTL_ADD, c->fd, &ee) == -1) 
        return 0;

    c->read->active = 1;
    // c->write->active = 1; // enable when write callback is done

    return 1;
}

// delete connection from epoll
// important: when a fd is closed, epoll will automatically delete it 
// so we do not need to explicitly call this function after close(fd)
int Epoll::epollDeleteConn(int ep, event_t *ev){
    connection_t *c;
    struct epoll_event ee;

    c = (connection_t*) ev->data;
    ee.events = 0;
    ee.data.ptr = NULL;

    if (epoll_ctl(ep, EPOLL_CTL_DEL, c->fd, &ee) == -1)
        return 0;

    c->read->active = 0;
    c->write->active = 0;

    return 1;
}
