#include <iostream>
#include <Server.h>
#include <MasterProcess.h>
#include <Core.h>

int main(int argc, char *argv[])
{
    int port = 8080;
    cycle_t cycle;
    
    Server myHTTPServer;
    myHTTPServer.start(port);
    cycle = myHTTPServer.serverInit();
    
    MasterProcess masterProcess;
    masterProcess.startMasterProcess(&cycle);

    return 0;
}

