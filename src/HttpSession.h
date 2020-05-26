#pragma once
#include <Core.h>
#include <HttpCodec.h>
#include <sys/types.h>
#include <sys/socket.h>

class HttpCodec;
// manage transactions and transaction handlers
class HttpSession
{
public:
    HttpSession(std::unique_ptr<HttpCodec> codec);
    
    void httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD);

private:
    std::unique_ptr<HttpCodec> codec_;
};

