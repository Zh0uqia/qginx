#include <iostream>
#include <WorkerProcess.h>

WorkerProcess::WorkerProcess(void *data, cycle_t* cycle)
    : heldLock(0),
    acceptEvent(1)
{
     // initiate connection and events
    ls = cycle->listening;
    c = cycle->connection;
    ls->connection = c;

    // initiate cycle 
    auto acceptHandler = std::bind(&Handler::acceptEventHandler, \
                            &handler, std::placeholders::_1, \
                            std::placeholders::_2);
    
    cycle->read_event = new event_t;
    cycle->write_event = new event_t;

    rev = cycle->read_event;
    wev = cycle->write_event;

    //initiate connection 
    c->read=rev;
    c->write=wev;

    // initiate read and write events 
   
    rev->handl = acceptHandler; // fatal
    rev->accept=1; // fatal
    rev->active=0; // fatal 
    rev->data=c;
    wev->data=c;

}

WorkerProcess::~WorkerProcess(){
    delete rev;
    delete wev;
    
    rev = NULL;
    wev = NULL;
}

void WorkerProcess::workerProcessCycle(void *data, cycle_t* cycle, struct mt* shmMutex){
    dbPrint("Worker process cycle of [process] " << getpid() << std::endl);
    workerProcessInit(data, cycle);
            
    for (int j=0; j<100; j++){
    // for ( ;; ){
        processEvents(data, cycle, shmMutex);
                        
    }
      
}

void WorkerProcess::workerProcessInit(void *data, cycle_t* cycle){
    // initiate epoll 
    epollFD = epl.epollInit();
    
    if (epollFD == -1)
        return;

}

void WorkerProcess::processEvents(void *data, cycle_t* cycle, struct mt* shmMutex){
   //  uintptr_t flags; // if POST_EVENT or not 

    if (trylockAcceptMutex(data, cycle, shmMutex) == 0){
        return;
    }
/*
    if (heldLock == 1)
        flags = POST_EVENT;
*/  
    getEventQueue(cycle);
    processPostedEvent(cycle, postedAcceptEvents);

    if (heldLock == 1)
        pthread_mutex_unlock(&shmMutex->mutex);

    processPostedEvent(cycle, postedDelayEvents);
}

int WorkerProcess::trylockAcceptMutex(void *data, cycle_t*cycle, struct mt* shmMutex){
    // get mutexI
    if (pthread_mutex_trylock(&shmMutex->mutex) == 0){
        dbPrint("Worker " << getpid() << " got the mutex." << std::endl);
        // this process get the lock and does not held the lock 
        if (heldLock == 0){
            if (enableAcceptEvent(cycle) == 0){
                pthread_mutex_unlock(&shmMutex->mutex);
                return 0;
            }
        }

        heldLock = 1;
        return 1;
    }
    
    // if did not get the lock, but hold the lock in last round, then unlock it
    if (heldLock == 1){
        connection_t *c;
        c = cycle->connection;

        if (epl.epollDeleteEvent(epollFD, c->read, READ_EVENT, DISABLE_EVENT) == 0){
            std::perror("Deleting the accept event");
            return 0;
        }
        heldLock = 0;
    }

    return 1;
}

int WorkerProcess::enableAcceptEvent(cycle_t *cycle){
    connection_t *c;
    listening_t *ls;

    ls = cycle->listening;
    c = ls->connection;

    // add accept event to epoll 
    if (epl.epollAddEvent(epollFD, c->read, READ_EVENT, 0) == 0){
        std::perror("Adding the accept event");
        return 0;
    }

    return 1;
}

void WorkerProcess::getEventQueue(cycle_t *cycle){
    struct epoll_event ee;
    event_t *rev, *wev;
    uint32_t revent, wevent;
    connection_t *c;
    
    struct epoll_event* eventList = (struct epoll_event*) calloc(MAX_EPOLLFD, \ 
                                                                 sizeof(event));
    
    int n = epoll_wait(epollFD, eventList, MAX_EPOLLFD, EPOLL_TIMEOUT);

    if (n == 0)
        dbPrint("No event is in the event wait list" << std::endl);

    int i;

    for (i=0; i<n; i++){
        ee = eventList[i];
    
        c = (connection_t*) ee.data.ptr;
        rev = c->read;
        revent = ee.events;

        if ((revent & EPOLLIN) && rev->active) {
            if (rev->accept == 1)
                postedAcceptEvents.push(rev);
            else
                postedDelayEvents.push(rev);
        }
        
        wev = c->write;
        if ((revent & EPOLLOUT) && wev->active) {
            postedDelayEvents.push(wev);
        }
    }
    free(eventList);
}

void WorkerProcess::processPostedEvent(cycle_t* cycle, \
                                       std::queue<event_t*> arr){
    event_t* cur;

    while (!arr.empty()){
        cur = arr.front();
        arr.pop();
        cur->handl(cur, epollFD);
    }
}
