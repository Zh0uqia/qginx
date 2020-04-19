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
        prev = EPOLLIN|EPOLLRDHUP;
    }

    if (e->active == 1) {
        op = EPOLL_CTL_MOD;
        events |= prev;
    } else if (e->active < 0) {
        op = EPOLL_CTL_ADD; // accept event 
    }
    
    ee.events = events | (uint32_t) flags;
    ee.data.ptr = c;
    // ee.data.fd = c->fd; // fatal error! epoll_data is a union, not a struct 

    dbPrint("op code is: "<< EPOLL_CTL_ADD << " " << EPOLL_CTL_MOD << " " << EPOLL_CTL_DEL << std::endl);
    dbPrint("Prepare to add event: fd " << c->fd << " op " << op << " event " \
            << ee.events << std::endl);

    if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
        return 0;

    }
    
    dbPrint("Epoll add event success: fd " << c->fd << " op " << op << " event " \
            << ee.events << std::endl);
    ev->active = 1;

    return 1;

}

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
        prev = EPOLLIN|EPOLLRDHUP;
    }

    if (e->active == 1) {
        op = EPOLL_CTL_MOD;
        ee.events = prev | (uint32_t) flags;
        ee.data.ptr = c;

    } else if (e->active < 0) {
        op = EPOLL_CTL_DEL;
        ee.events = 0;
        ee.data.ptr = NULL;
    }
    
    dbPrint("Prepare to delete event: fd " << c->fd << " op " << op << " event " \
            << ee.events << std::endl);

    if (epoll_ctl(ep, op, c->fd, &ee) == -1){
        return 0;
    }
    
    dbPrint("Epoll delete event success: fd " << c->fd << " op " << op << " event " \
            << ee.events << std::endl);

    ev->active = 0;
        
    return 1;
}
