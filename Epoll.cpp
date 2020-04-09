#include <iostream>
#include <Epoll.h>

int Epoll::epollInit(){
    epollFd_ = epoll_create(0);

    if (epollFd_ < 0) {
        perror("epoll_create");
        return -1;
    }

    return epollFd_;
}

int Epoll::epollAddListenEvent(int s_fd){
    struct epoll_event listenEvent;
    listenEvent.data.fd = s_fd;
    listenEvent.events = EPOLLIN;
    
    return epoll_ctl(epollFd_, EPOLL_CTL_ADD, s_fd, &listenEvent);
}

int Epoll::epollAddEvent(){

}
