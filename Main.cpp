#include <iostream>
#include <Server.h>
#include <MasterProcess.h>

int main(int argc, char *argv[])
{
    int port = 8080;
    int serverFD;
    
    Server myHTTPServer;
    myHTTPServer.start(port);
    serverFD = myHTTPServer.serverInit();
    
    MasterProcess masterProcess;
    masterProcess.startMasterProcess(serverFD);

    return 0;
}

