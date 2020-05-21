#pragma once
#include <httpServer/RequestHandlerFactory.h>

// Configuration options for http servers 
class HttpServerOptions{
public:

    int workerNumber = 1;

    int listenBacklog = 1024;

    // a vector of request handler factories 
    std::vector<RequestHandlerFactory> handlerFactories;
};
