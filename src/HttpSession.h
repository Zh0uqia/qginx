#pragma once
#include <Core.h>
#include <HttpCodec.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <HttpMessage.h>
#include <Handler.h>
#include <SimpleController.h>

class HttpCodec;
// manage transactions and transaction handlers
class HttpSession
{
public:
    HttpSession(std::unique_ptr<HttpCodec> codec);
    
    void httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD);

    void onMessageBegin(HttpMessage* msg);
    void onHeadersComplete(HttpMessage* msg);
    
    connection_t* getConn();

private:
    connection_t* conn_;
    std::unique_ptr<HttpCodec> codec_;
    SimpleController controller_;
};

