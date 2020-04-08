#pragma once
#include <conf.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstring> 
#include <iostream>
#include <MasterProcess.h>
#include <Mutex.h>
#include <Epoll.h>

class WorkerProcess 
{
public:
    /*
    typedef struct {
        pid_t pid;
        int status;
        spawn_proc_pt proc;
        void *data;
                                                
    }process_t;
    */

    // process_t processes[MAX_PROCESSES];

    void workerProcessInit(void *data, int serverFD);

    void processEvents(void *data, int serverFD, struct mt* shmMutex);

    void workerProcessCycle(void *data, int serverFD, struct mt* shmMutex);

private:
    int epollFD;
    struct epoll_event* events;
};


