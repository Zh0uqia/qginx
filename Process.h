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
    typedef void (*spawn_proc_pt) (pid_t pid, void *data);
    
    typedef struct {
        pid_t pid;
        int status;
        spawn_proc_pt proc;
        void *data;
    }process_t;

    static process_t processes[MAX_PROCESSES];

    void startMasterProcess();
    
    void startSingleProcess();

    void startWorkerProcess(int n);
   
    static void workerProcessInit(pid_t pid, void *data);

    static void processEvents(void *data);

    pid_t spawnProcess(spawn_proc_pt proc, void *data);

    static void workerProcessCycle(pid_t pid, void *data);

    static void mutexInit();

private:
    struct mt{
        int num;
        pthread_mutex_t mutex;
        pthread_mutexattr_t mutexattr;
    };

    inline static struct mt* mm;
};

