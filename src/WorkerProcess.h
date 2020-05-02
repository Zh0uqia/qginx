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
#include <queue>
#include <signal.h>

#define POST_EVENT 2

class WorkerProcess 
{
public:
    WorkerProcess(void*, cycle_t*);
    ~WorkerProcess();

    /*
    typedef struct {
        pid_t pid;
        int status;
        spawn_proc_pt proc;
        void *data;
                                                
    }process_t;
    */

    // process_t processes[MAX_PROCESSES];
    
    connection_t* getConnection(cycle_t* cycle, int sFD);
 
    void workerProcessInit(void *data, cycle_t* cycle);

    void processEvents(void *data, cycle_t* cycle, struct mt* shmMutex);

    void workerProcessCycle(void *data, cycle_t* cycle, struct mt* shmMutex);

    int trylockAcceptMutex(void *data, cycle_t*cycle, struct mt* shmMutex);
    int enableAcceptEvent(cycle_t *cycle);
    void getEventQueue(cycle_t *cycle, int timer, uintptr_t flags);
    void processPostedEvent(cycle_t* cycle, std::queue<event_t*>&);
    
    static void handleSigpipe(int);
 
private:
    Epoll epl;
    int epollFD;
    struct epoll_event event;
    struct epoll_event* events;
    struct epoll_event* eventList;
    std::queue<event_t*> postedAcceptEvents;
    std::queue<event_t*> postedDelayEvents;

    int heldLock;

    Handler handler;

    event_t *cycleReadEvent, *cycleWriteEvent;
    connection_t *cycleConnection, *conn;
    listening_t *ls;

};


