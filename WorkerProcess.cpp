#include <iostream>
#include <WorkerProcess.h>

WorkerProcess::WorkerProcess()
    : heldLock(0),
    acceptEvent(1)
{}

void WorkerProcess::workerProcessCycle(void *data, cycle_t* cycle, struct mt* shmMutex){
    workerProcessInit(data, cycle);
            
    for (int j=0; j<20; j++){
        processEvents(data, cycle, shmMutex);
                        
    }
        
}

void WorkerProcess::workerProcessInit(void *data, cycle_t* cycle){
    event_t *rev, *wev;
    listening_t *ls;
    connection_t *c;

    // initiate epoll 
    epollFD = epl.epollInit();
    
    if (epollFD == -1)
        return;

    // initiate connection and events
    ls = cycle->listening;
    c = cycle->connection;
    ls->connection = c;

    // initiate cycle 
    cycle->read_event = (event_t*) malloc(sizeof(event_t));
    cycle->write_event = (event_t*) malloc(sizeof(event_t));

    rev = cycle->read_event;
    wev = cycle->write_event;

    //initiate connection 
    c->read=rev;
    c->write=wev;

    // initiate read and write events 
    auto acceptHandler = std::bind(&Handler::acceptEventHandler, \
                            &handler);
    rev->handler = acceptHandler;
    rev->accept=1;
    rev->data=c;
    wev->data=c;

    events = (struct epoll_event*) calloc(MAX_EPOLLFD, \ 
                                          sizeof(struct epoll_event));
    
    /*
    // all worker processes put listening event into epoll during initiation
    if (epl.epollAddEvent(rev, READ_EVENT, 0) == 0)
        std::perror("Add listening event");
    */ 
}

void WorkerProcess::processEvents(void *data, cycle_t* cycle, struct mt* shmMutex){
    int flags; // if POST_EVENT or not 

    if (trylockAcceptMutex(data, cycle, shmMutex)){

    }
    

}

int WorkerProcess::trylockAcceptMutex(){
    // get mutexI
    if (pthread_mutex_trylock(&shmMutex->mutex) == 0){
        dbPrint("Worker " << data << " got the mutex." << std::endl);
        
        // this process get the lock and does not held the lock 
        if (heldLock == 0){
            if (enableAcceptEvent(cycle) == 0){
                pthread_mutex_unlock(&shmMutex->mutex);
                return 0;
            }
            heldLock = 1;
        }
        
    else{
        // if did not get the lock, but hold the lock in last round, then unlock it 
        if (heldLock = 1){
            if (epl.epollDeleteEvent(c->read, READ_EVENT, 0) == 0)
                std::perror("Deleting the accept event");
                return 0;
            heldLock = 0;
        }
    }
}

int WorkerProcess::enableAcceptEvent(cycle_t *cycle){
    connection_t *c;
    listening_t *ls;

    ls = cycle->listening;
    c = ls->connection;

    if (epl.epollAddEvent(c->read, READ_EVENT, 0) == 0)
        return 0;

    return 1;
}
