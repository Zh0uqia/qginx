#include <iostream>
#include <WorkerProcess.h>

void WorkerProcess::workerProcessCycle(void *data, int serverFD, struct mt* shmMutex){
    // workerProcessInit(data, serverFD);
            
    for (int j=0; j<20; j++){
        processEvents(data, serverFD, shmMutex);
                        
    }
        
}

void WorkerProcess::processEvents(void *data, int serverFD, struct mt* shmMutex){
    // get mutex 
    pthread_mutex_lock(&shmMutex->mutex);
    dbPrint("Worker " << data << " got the mutex." << std::endl);
    pthread_mutex_unlock(&shmMutex->mutex);

    // add events to epoll
    

}


