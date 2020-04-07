#pragma once
#include <conf.h>
#include <functional>
#include <Process.h>
#include <WorkerProcess.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <Mutex.h>

class MasterProcess
{
public:
    void startMasterProcess(int serverFD);
    void startWorkerProcess(int n, int serverFD);
    void mutexInit();

private:
    int mutexShmID;

    struct mt* mutexShmPTR;
    struct mt* mm;


};

