#pragma once
#include <conf.h>
#include <unordered_set>
#include <Core.h>
#include <unistd.h>
// #include <Dispatcher.h>

enum ProcessState{
    STATE_STATUSLINE = 1,
    STATE_HEADER,
    STATE_BODY,
    STATE_FINISH,
    STATE_ERROR 
};

enum StatusState{
    STATUS_FINISH = 1,
    STATUS_ERROR 
};

enum HeaderState{
    HEADER_FINISH = 1,
    HEADER_ERROR
};

enum BodyState{
    BODY_FINISH = 1,
    BODY_ERROR
};

enum MethodState{
    METHOD_GET_STATIC = 1,
    METHOD_GET_DYNAMIC,
    METHOD_POST,
    METHOD_OTHER,
    METHOD_ERROR
};

enum HttpVersionState{
    HTTP_10 = 1,
    HTTP_11,
    VERSION_ERROR
};

class RequestHandler
{
public:
    RequestHandler();
    
    ProcessState getState();
    MethodState getMethod();
    HttpVersionState getVersion();
    std::string getUri();
    std::string getQueryString();
    std::string getBody();

    void httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD);
    void processRequest(std::string);
 
private:
    StatusState processStatus(std::string);
    HeaderState processHeader(std::string);
    BodyState processBody(std::string);

    ProcessState state_;
    MethodState method_;
    HttpVersionState httpVersion_;
    std::string uri_, queryString_, body_;
    
    size_t nowReadPos_;
    std::unordered_set<std::string> methodSet = {"HEAD", "PUT", \
                    "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
};

