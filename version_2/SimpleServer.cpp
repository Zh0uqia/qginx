#include <iostream>

#include <conf.h>
#include <StaticHandler.h>
#include <httpServer/HttpServer.h>

int main(int argc, char* argv[])
{
    // different ports are for different protocals
    // can expand this to be a vector 
    HttpServer::IPConfig IPs = {
        IP_ADDRESS, HTTP1_PORT, HttpServer::Protocal::HTTP};

    HttpServerOptions options;
    options.workerNumber = WORKER_NUMBER;

    // add request handler factories here 
    
    HttpServer server(options);
    server.bindPort(IPs);

    server.start();

    return 0;
}

