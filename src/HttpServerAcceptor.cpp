#include <iostream>
#include <HttpServerAcceptor.h>

int HttpServerAcceptor::setNonBlock(int fd){
    int flags;

    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags=0;

    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

connection_t* HttpServerAcceptor::getConnection(cycle_t* cycle, int sFD){
    connection_t* c;
    event_t *revent, *wevent;

    c = cycle->free_connections;

    cycle->free_connections = (connection_t*) c->data; // next node 
    cycle->free_connections_n--;

    revent = c->read;
    wevent = c->write;
    
    c->read = revent;
    c->write = wevent;
    c->fd = sFD;

    memset(revent, 0, sizeof(event_t));
    memset(wevent, 0, sizeof(event_t));
    revent->data = c;
    wevent->data = c;

    return c;
}


void HttpServerAcceptor::acceptEventHandler(cycle_t* cycle, event_t* ev, int epollFD){

    listening_t *ls;
    connection_t *c, *conn;
    int listenFD;

    c = (connection_t*) ev->data;
    ls = c->listening;
    listenFD = ls->fd;

    struct sockaddr_in addr;
    int addrlen = sizeof(addr);

    int acceptFD = accept(listenFD, (struct sockaddr *)&addr, \
                          (socklen_t*)&addrlen);
    
    /*
    if (acceptFD == -1){
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            break;
        }else{
            std::perror("Accept error");
            return;
        }
    }
    */

    if (acceptFD == -1)
        return;

    cycle->accept_disabled = cycle->total_connection/8 - cycle->free_connections_n; 
    
    if (setNonBlock(acceptFD) < 0){
        std::perror("Set accept fd nonblock failed");
        return;
    }
    
    conn = getConnection(cycle, acceptFD); // get a free connection; bind acceptFD with it 
    conn->listening = ls;

    httpInitConnection(conn, epollFD);

}

// set callback function for read event 
void HttpServerAcceptor::httpInitConnection(connection_t *c, int epollFD){
    event_t *rev;
    rev = c->read;

    // change handler of read and write events 
    auto waitRequestHandler = std::bind(&Response::httpWaitRequestHandler, \
                            &rp, std::placeholders::_1, \
                            std::placeholders::_2, std::placeholders::_3);
    
    rev->handl = waitRequestHandler;
    // c->write->handler = httpEmptyHandler;
    
    // add to epoll to check whether there are data to be read 
    if (epl.epollAddEvent(epollFD, c->read, READ_EVENT, 0) == 0)
        std::perror("Adding read event");

    setTransport(conn);
}

void HttpServerAcceptor::setTransport(connection_t *c){
    
}
