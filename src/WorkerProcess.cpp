#include <iostream>
#include <WorkerProcess.h>

WorkerProcess::WorkerProcess(void *data, cycle_t* cycle)
    : heldLock(0)
{
     // initiate connection and events
    ls = cycle->listening;
    // initiate cycle 
   
    cycle->read_event = (event_t*) calloc(MAX_CONNECTIONS, sizeof(event_t));
    cycle->write_event = (event_t*) calloc(MAX_CONNECTIONS, sizeof(event_t));
    cycle->connection = (connection_t*) calloc(MAX_CONNECTIONS, sizeof(connection_t));
    if (cycle->connection == NULL || cycle->read_event == NULL \
        || cycle->write_event == NULL)
        std::perror("Connection of cycle is NULL"); 
    
    cycleReadEvent = cycle->read_event;
    cycleWriteEvent = cycle->write_event;
    cycleConnection = cycle->connection;

}

WorkerProcess::~WorkerProcess(){
    free(cycleReadEvent);
    free(cycleWriteEvent);
    free(cycleConnection);

    cycleReadEvent = NULL;
    cycleWriteEvent = NULL;
    cycleConnection = NULL;
}

void WorkerProcess::workerProcessCycle(void *data, cycle_t* cycle, struct mt* shmMutex){
    workerProcessInit(data, cycle);
            
    // for (int j=0; j<5000; j++){
    for ( ;; ){
        processEvents(data, cycle, shmMutex);
        
    }
      
}

void WorkerProcess::workerProcessInit(void *data, cycle_t* cycle){
    // initiate epoll 
    epollFD = epl.epollInit();
    
    if (epollFD == -1)
        return;

    // initialize free connections linked list 
    int i = MAX_CONNECTIONS;
    connection_t *next, *c;
    event_t *rev; //*wev;

    next = NULL;
    c = cycle->connection;

    do{
        i--;
        
        c[i].data = next;
        c[i].read = &cycle->read_event[i];
        c[i].write = &cycle->write_event[i];

        next = &c[i];
    }while(i);

    cycle->free_connections = next;
    cycle->free_connections_n = MAX_CONNECTIONS;

    cycle->total_connection = MAX_CONNECTIONS;
    cycle->accept_disabled = cycle->total_connection/8 - cycle->free_connections_n; 
    
    // initialize conncection, read and write events
    conn = getConnection(cycle, ls->fd); // get a free connection 
    conn->listening = ls;
    ls->connection = conn;

    rev = conn->read;
    rev->accept = 1;
    rev->active = 0;

    auto acceptHandler = std::bind(&Handler::acceptEventHandler, \
                            &handler, std::placeholders::_1, \
                            std::placeholders::_2, std::placeholders::_3);
    rev->handl = acceptHandler;

    // wev = conn->write;

    // if do not use mutex, add listen fd to every process 
    if (ACCEPT_MUTEX == 0)
        if (epl.epollAddEvent(epollFD, conn->read, \
                              READ_EVENT, EPOLLEXCLUSIVE) == 0){
            std::perror("Adding the exclusive accept event");
    }
}

connection_t* WorkerProcess::getConnection(cycle_t* cycle, int sFD){
    connection_t* c;
    event_t *revent, *wevent;

    c = cycle->free_connections;

    cycle->free_connections = (connection_t*) c->data; // next node 
    cycle->free_connections_n--;

    revent = c->read;
    wevent = c->write;
    
    c->read = revent;
    c->write = wevent;
    c->fd = sFD;

    // reset revent and wevent because we did not do it in freeConnection()
    memset(revent, 0, sizeof(event_t));
    memset(wevent, 0, sizeof(event_t));
    revent->data = c;
    wevent->data = c;

    return c;
}
/*
void WorkerProcess::handleSigpipe(){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL) return;
}
*/

void WorkerProcess::handleSigpipe(int signum){
    std::cout << "Caught signal SIGPIPE " << signum << std::endl;
    return;
}


void WorkerProcess::processEvents(void *data, cycle_t* cycle, struct mt* shmMutex){
    uintptr_t flags; // if POST_EVENT or not 
    int timer; // timeout for epoll_wait()

    // handleSigpipe();
    signal(SIGPIPE, handleSigpipe);

    timer = EPOLL_TIMEOUT; //reset timer every loop 
    flags = 0; // reset flags every loop 

    if (ACCEPT_MUTEX){
        if (cycle->accept_disabled > 0){
            cycle->accept_disabled--;
        }else{
            if (trylockAcceptMutex(data, cycle, shmMutex) == 0){
                return;
            }
            if (heldLock == 1)
                flags |= POST_EVENT;
            else{
                if (timer == EPOLL_TIMEOUT || timer>MUTEX_DELAY)
                    timer = MUTEX_DELAY;
            }
        }
    }

    getEventQueue(cycle, timer, flags);
    processPostedEvent(cycle, postedAcceptEvents);

    if (heldLock == 1){
        // might failed if current process does not own the mutex
        if (pthread_mutex_unlock(&shmMutex->mutex) != 0)
            std::perror("Unlocking");
    }
    processPostedEvent(cycle, postedDelayEvents);
    

}

int WorkerProcess::trylockAcceptMutex(void *data, cycle_t*cycle, struct mt* shmMutex){
    // get mutex
    int getLock;

    if ((getLock = pthread_mutex_trylock(&shmMutex->mutex)) == 0){
        // this process get the lock and does not held the lock 
        // dbPrint("Worker " << data << " got the lock " << std::endl);
        if (heldLock == 0){
            if (enableAcceptEvent(cycle) == 0){
                pthread_mutex_unlock(&shmMutex->mutex);
                return 0;
            }
        }

        heldLock = 1;
        return 1;
    }else if (getLock == EBUSY){
        // if did not get the lock, but hold the lock in last round, then unlock it
       if (heldLock == 1){
            if (epl.epollDeleteEvent(epollFD, conn->read, READ_EVENT, DISABLE_EVENT) == 0){
                std::perror("Deleting the accept event");
                return 0;
            }
            // dbPrint("------------Process " << data << " delete fd-----------" << std::endl);
            heldLock = 0;
        }

        return 1;
    }else{
        std::perror("Getting the mutex lock");
        return 0;
    }
}

int WorkerProcess::enableAcceptEvent(cycle_t *cycle){
    // add accept event to epoll 
    if (epl.epollAddEvent(epollFD, conn->read, READ_EVENT, 0) == 0){
        std::perror("Adding the accept event");
        return 0;
    }

    return 1;
}

void WorkerProcess::getEventQueue(cycle_t *cycle, int timer, uintptr_t flags){
    struct epoll_event ee;
    event_t *rev, *wev;
    uint32_t revent;
    connection_t *c;
    
    struct epoll_event* eventList = (struct epoll_event*) \
                                    calloc(MAX_EPOLLFD, sizeof(event));
    
    int n = epoll_wait(epollFD, eventList, MAX_EPOLLFD, timer);
    if (n==-1)
        std::perror("Epoll wait error");

    for (int i=0; i<n; i++){
        ee = eventList[i];
    
        c = (connection_t*) ee.data.ptr;
        rev = c->read;
        revent = ee.events;

        if ((revent & EPOLLIN) && rev->active) {
            if (flags & POST_EVENT){
                if (rev->accept == 1)
                // if POST_EVENT, delay processing time-consuming events
                // until release lock 
                    postedAcceptEvents.push(rev);
                else
                    postedDelayEvents.push(rev);  
            }else
                // process read/write events right now 
                rev->handl(cycle, rev, epollFD);
   
        }
        
        wev = c->write;
        if ((revent & EPOLLOUT) && wev->active) {
            if (flags & POST_EVENT)
                postedDelayEvents.push(wev);
            else
                wev->handl(cycle, wev, epollFD);
        }
    }
    
    free(eventList);
}

void WorkerProcess::processPostedEvent(cycle_t* cycle, \
                                       std::queue<event_t*>& arr){
    event_t* cur;

    while (!arr.empty()){
        cur = arr.front();
        arr.pop();
        cur->handl(cycle, cur, epollFD);
    }

}
