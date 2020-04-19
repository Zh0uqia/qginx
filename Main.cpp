#include <iostream>
#include <Server.h>

int main(int argc, char *argv[])
{
    Server myHTTPServer;
    myHTTPServer.start(PORT);
    myHTTPServer.serverInit();
    // myHTTPServer.handNewConn(); // switch to single process version 

    return 0;
}

