#pragma once
#include <conf.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstring> 
#include <iostream>

class Process
{
public:
    typedef void (*spawn_proc_pt) ();

    void startMasterProcess();
    
    void startSingleProcess();

    void startWorkerProcess(int n);
    
    void spawnProcess();

    void workerProcessCycle();

    void mutexInit();

private:
    
};

