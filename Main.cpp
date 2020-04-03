#include <iostream>
#include <Server.h>
#include <Process.h>

int main(int argc, char *argv[])
{
    int port = 8080;
    /*
    Server myHTTPServer;
    myHTTPServer.start(port);

    myHTTPServer.handNewConn();
    */
    Process masterProcess;
    masterProcess.startMasterProcess();

    return 0;
}

