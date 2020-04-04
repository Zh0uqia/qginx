#include <iostream>
#include <Process.h>


void Process::startMasterProcess(){
    mutexInit();
    startWorkerProcess(WORKER_NUMBER);

}

void Process::startWorkerProcess(int n){
    int i;
    pid_t pid;

    for (i=0; i<n; i++){
        pid = spawnProcess(workerProcessCycle, (void *) (intptr_t) i);
        if (pid == 0)
            break;
    }
}

pid_t Process::spawnProcess(spawn_proc_pt proc, void *data){
    pid_t pid;

    pid = fork();

    switch(pid){

    case -1:
        perror("Failure of forking a child process: ");
        return pid;

    case 0:
        // if fork() success, start worker process 
        dbPrint( "[son] " << getpid() << " from [parent] " << getppid() << "\n" << std::endl);
        proc(pid, data);
        break;

    default:
        break;
    }
    
    return pid;
}

void Process::workerProcessCycle(pid_t pid, void *data){
    workerProcessInit(pid, data);
    
    for (int j=0; j<20; j++){
        processEvents(data);
    }
}

// initiate epoll 
void Process::workerProcessInit(pid_t pid, void *data){
    int s;
    /*
    for (s=0; s<MAX_PROCESSES; s++){
        if (processes[s].pid==-1){
            printf("s is %d\n", s);
            break;
        }
    }
    */ 
    /*
    printf("here is test \n");
    processes[s].pid=pid;
    processes[s].data=data;
    */
    // Epoll ee;

    // ee.epollInit();

}

void Process::processEvents(void *data){
    // get mutex 
    pthread_mutex_lock(&Process::mm->mutex);
    dbPrint("Worker " << data << " got the mutex." << std::endl);
    pthread_mutex_unlock(&Process::mm->mutex);
    // add events to epoll
        
}

void Process::mutexInit(){
    mm = (mt*) mmap(NULL, sizeof(*mm), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    memset(mm ,0x00, sizeof(*mm));
        
    pthread_mutexattr_init(&mm->mutexattr);
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mm->mutex, &mm->mutexattr);

}
