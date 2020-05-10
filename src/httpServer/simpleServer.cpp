#include <iostream>

int main(int argc, char* argv[])
{
    // different ports are for different protocals
    std::vector<> IPs = {};

    httpServerOptions options;
    // add request handler factories here 
    
    httpServer server;
    server.bind();

    server.start();
    masterProcess.start();

    return 0;
}

