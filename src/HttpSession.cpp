#include <iostream>
#include <HttpSession.h>

HttpSession::HttpSession(std::unique_ptr<HttpCodec> codec, connection_t* c)
    : conn_(c),
    codec_(std::move(codec))
{
    codec_ -> setCallback(this);
}

void HttpSession::httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD){
    int nread, readSum;

    nread = 0;
    readSum = 0;
    
    std::string inBuffer;

    while (1){
        char buf[BUFFERLENGTH];
        memset(buf, 0, sizeof(buf));

        if ((nread = read(conn_->fd, buf, BUFFERLENGTH)) < 0){
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN)
                break;
            else{
                std::perror("Read from socket");
                return;
            }
        }else if (nread == 0){
            return;
        }else{
            readSum += nread;
            inBuffer += std::string(buf, buf+nread);
        }
    }

    /* Echo test */
    /*
    std::string outBuffer_;
    outBuffer_ += "HTTP/1.1 " + std::to_string(220) + " OK" + "\r\n";
    outBuffer_ += "Content-Type: text/html\r\n";
            // outBuffer_ += "Connection: Close\r\n";
    outBuffer_ += "Content-Length: " + std::to_string(readSum) + "\r\n";
    outBuffer_ += "Server: Qginx\r\n";
    outBuffer_ += "\r\n";
    outBuffer_ += inBuffer;

    if (write(conn_->fd, outBuffer_.c_str(), outBuffer_.size()) < 0)
        std::perror("Write to socket");
    */

    // begin processing http requests
    codec_ -> onIngress(const_cast<char*>(inBuffer.c_str()), readSum);
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
