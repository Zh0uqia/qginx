#include <iostream>
#include <Response.h>
#include <bits/stdc++.h>

void Response::httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD){
    connection_t *c;
    char readBuffer[BUFFERLENGTH];

    memset(readBuffer, 0, sizeof(readBuffer));
    c = (connection_t*) ev->data;
    read(c->fd, readBuffer, BUFFERLENGTH); // read request from the socket 

    std::string cmd(readBuffer);

    RequestHandler requestHandler;
    requestHandler.processRequest(cmd);
    
    ProcessState ps = requestHandler.getState();
    
    if (ps == STATE_FINISH){
        Dispatcher dsp;
        std::string response;
        response = dsp.dispatch(requestHandler);

        char send_buff[1024];
        char content_buff[1024];

        std::string header_buff;
        header_buff = "HTTP/1.1 " + std::to_string(220) + " OK" + "\r\n";
        header_buff += "Content-Type: text/html\r\n";
        // header_buff += "Connection: Close\r\n";
        header_buff += "Content-Length: " + std::to_string(response.size()) + "\r\n";
        header_buff += "Server: Qginx\r\n";
        header_buff += "\r\n";

        sprintf(send_buff, "%s", header_buff.c_str());
        sprintf(content_buff, "%s", response.c_str());

        write(c->fd, send_buff, strlen(send_buff));

        write(c->fd, content_buff, strlen(content_buff));
        
        dbPrint(cycle->free_connections_n << std::endl);

    }else if (cmd == ""){ // when client close connection, will receive 0 byte 
        freeConnection(cycle, c, epollFD);
        std::cout << "closed connection" << std::endl;
    }
    
}

void Response::freeConnection(cycle_t *cycle, connection_t *c, int epollFD){
    if (epl.epollDeleteEvent(epollFD, c->read, READ_EVENT, DISABLE_EVENT) == 0)
        std::perror("Deleting read event");

    if (close(c->fd) == -1)
        std::perror("Closing connection fd");

    // put back to free connections
    c->data = cycle->free_connections;
    cycle->free_connections = c;
    cycle->free_connections_n++;

    // reset c
    c->fd = 0;
        
}
