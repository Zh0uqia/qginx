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
#include <Core.h>
#include <Handler.h>

class WorkerProcess 
{
public:
    WorkerProcess();

    /*
    typedef struct {
        pid_t pid;
        int status;
        spawn_proc_pt proc;
        void *data;
                                                
    }process_t;
    */

    // process_t processes[MAX_PROCESSES];

    void workerProcessInit(void *data, cycle_t* cycle);

    void processEvents(void *data, cycle_t* cycle, struct mt* shmMutex);

    void workerProcessCycle(void *data, cycle_t* cycle, struct mt* shmMutex);

private:
    Epoll epl;
    int epollFD;
    struct epoll_event event;
    struct epoll_event* events;

    int heldLock;
    int acceptEvent;

    Handler handler;
};


