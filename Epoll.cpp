#include <iostream>
#include <Epoll.h>

int epollInit(){
    epollFd_ = epoll_create();

    if (epollFd_ < 0) {
        perror("epoll_create");
        return -1;
    }

    return epollFd_;
}

void epollAddEvent(){

}

