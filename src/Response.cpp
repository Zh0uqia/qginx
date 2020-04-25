#include <iostream>
#include <Response.h>
#include <bits/stdc++.h>

// edge trigger: read until nothing to read 
ssize_t Response::readn(int fd, std::string &inBuffer, bool &isZero){
    ssize_t nread, readSum;
   
    nread = 0;
    readSum = 0;

    while (1){
        char buff[BUFFERLENGTH];
        if ((nread = read(fd, buff, BUFFERLENGTH)) < 0){
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN){
                dbPrint("Read " << readSum << " bytes " << std::endl);
                break;
            }else{
                std::perror("Read from socket");
                return -1;
            }
        }else if (nread == 0){ // nothing in read socket, client may have closed
            isZero = true;
            dbPrint("Read 0 byte." << std::endl);
            break;
        }else{
            readSum += nread;
            inBuffer += std::string(buff, buff+nread);
        }
    }
    return readSum; 
}

// write until error exists 
ssize_t Response::writen(int fd, std::string& buf, size_t n){
    ssize_t nwritten, nleft, writeSum;
    nwritten = 0;
    writeSum = 0;
    nleft = n;

    const char* ptr = buf.c_str();
    while (nleft > 0){
        if ((nwritten = write(fd, ptr, nleft)) < 0){
            if (errno == EINTR){
                nwritten = 0;
                continue;
            }else if (errno == EAGAIN)
                break;
            else{
                std::perror("Write to socket");
                return -1;
            }
        }else if (nwritten == 0){
            dbPrint("Write 0 byte" << std::endl);
        }else{
            nleft -= nwritten;
            writeSum += nwritten;
            ptr += nwritten;
        }
    }
    if (writeSum == static_cast<int>(n))
        buf.clear();
    else
        buf = buf.substr(writeSum);

    return writeSum;
}

void Response::httpWriteHandler(cycle_t *cycle, event_t *ev, int epollFD){
    connection_t *c;
    c = (connection_t*) ev->data;

    if (writen(c->fd, outBuffer_, sizeof(outBuffer_)) < 0)
        return;
    if (outBuffer_.size() > 0){
        if (epl.epollAddEvent(epollFD, ev, WRITE_EVENT, 0) < 0)
            std::perror("Add write event");
    }
    return;
}

void Response::httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD){

    connection_t *c;
    c = (connection_t*) ev->data;

    bool isZero = false;
    if (readn(c->fd, inBuffer, isZero) == -1)
        return;
    else if (isZero == true){
        // dbPrint("free connection fd = " << c->fd << std::endl);
        // freeConnection(cycle, c, epollFD);
    }else{
        while (inBuffer.size() > 0){
            // inBuffer have chunks of request data
            // analyze the request data one by one
            // when have data to write, handleWrite()
            
            requestHandler.reset(); // reset instance for each request 
            httpRequestAnalyze(inBuffer);
            
            if (outBuffer_.size() > 0){ // have data to write
                httpWriteHandler(cycle, ev, epollFD);
            }
        }
    }
}

void Response::httpRequestAnalyze(std::string& inBuffer){
    requestHandler.processRequest(inBuffer);
    
    ProcessState ps = requestHandler.getState();
    
    if (ps == STATE_FINISH){
        Dispatcher dsp;
        std::string response;
        response = dsp.dispatch(requestHandler);

        outBuffer_ = "HTTP/1.1 " + std::to_string(220) + " OK" + "\r\n";
        outBuffer_ += "Content-Type: text/html\r\n";
        // outBuffer_ += "Connection: Close\r\n";
        outBuffer_ += "Content-Length: " + std::to_string(response.size()) + "\r\n";
        outBuffer_ += "Server: Qginx\r\n";
        outBuffer_ += "\r\n";
        outBuffer_ += response;
    
    }else{
        // std::cout << "analysis state error" << std::endl;
    }
}

// if client close connections 
// close connections to avoid too much opening files
void Response::freeConnection(cycle_t *cycle, connection_t *c, int epollFD){
    /*
    if (epl.epollDeleteConn(epollFD, c->read) == 0)
        std::perror("Deleting read event");
    */
    if (close(c->fd) == -1)
        std::perror("Closing connection fd");

    // put back to free connections
    c->data = cycle->free_connections;
    cycle->free_connections = c;
    cycle->free_connections_n++;

    // reset c
    c->fd = 0;
        
}
