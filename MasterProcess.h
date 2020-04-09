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
#include <Core.h>

class MasterProcess
{
public:
    void startMasterProcess(cycle_t*);
    void startWorkerProcess(cycle_t*);
    void mutexInit();

private:
    int mutexShmID;

    struct mt* mutexShmPTR;
    struct mt* mm;


};

