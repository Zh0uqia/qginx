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
    Epoll epl;
    int epollFD = epl.epollInit();
    
    if (epollFD == -1)
        return;

    // initiate connection and events
    c = cycle->connection;
    cycle->read_event = (event_t*) malloc(sizeof(event_t));
    cycle->write_event = (event_t*) malloc(sizeof(event_t));

    rev = cycle->read_event;
    wev = cycle->write_event;


    // all the worker processes would add listen port into epollFD  
    if (epl.epollAddListenEvent(cycle) < 0)
        std::perror("Add listen event to epoll");

    events = (struct epoll_event*) calloc(MAX_EPOLLFD, sizeof(struct epoll_event));


}

void WorkerProcess::processEvents(void *data, cycle_t* cycle, struct mt* shmMutex){
    
    

}

int WorkerProcess::trylockAcceptMutex(){
    // get mutexI
    if (pthread_mutex_trylock(&shmMutex->mutex) == 0){
        dbPrint("Worker " << data << " got the mutex." << std::endl);
        
        // if processing accept event, or already held lock, return 
        if (heldLock && acceptEvent == 0)
            return 1;
        

    }
    pthread_mutex_unlock(&shmMutex->mutex);


}

void WorkerProcess::enableAcceptEvent(){

}
