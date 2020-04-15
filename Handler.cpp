#include <iostream>
#include <Handler.h>

void Handler::acceptEventHandler(event_t* ev, int epollFD){
    dbPrint("Accept event handler started" << std::endl);

    int listenFD;
    connection_t *c;

    c = (connection_t*) ev->data;
    listenFD = c->fd;

    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    for (int i=0; i<5; i++){
        int acceptFD = accept(listenFD, (struct sockaddr *)&addr, \
                              (socklen_t*)&addrlen);
        if (acceptFD == -1){
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }else{
                std::perror("Accept error");
                return;
            }
        }
        char *pbuf = new char[1024];
        memset(pbuf, 0, 1024);

        int nread = read(acceptFD, pbuf, 1024);

    }
}
