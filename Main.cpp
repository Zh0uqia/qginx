#include <iostream>
#include <Server.h>

int main(int argc, char *argv[])
{
    int port = 8080;
    
    Server myHTTPServer;
    myHTTPServer.start(port);

    myHTTPServer.handNewConn();
    

    return 0;
}

