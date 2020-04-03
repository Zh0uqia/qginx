#include <iostream>
#include <Process.h>

void Process::startMasterProcess(){
    mutexInit();
    startWorkerProcess(WORKER_NUMBER);

}

void Process::startWorkerProcess(int n){
    int i;

    for (i=0; i<n; i++){
        spawnProcess(workerProcessCycle);
    }
}

void Process::spawnProcess(spawn_proc_pt proc){
    pid_t pid;

    pid = fork();

    switch(pid){

    case -1:
        perror("Failure of forking a child process: ");
        return;

    case 0:
        // if fork() success, start worker process 
        dbPrint( "[son] " << getpid() << " from [parent] " << getppid() << "\n" << std::endl);
        proc();
        exit(0);
        break;

    default:
        break;
    }
    
}

void Process::workerProcessCycle(){
    
}

void Process::mutexInit(){
    struct mt{
        int num;
        pthread_mutex_t mutex;
        pthread_mutexattr_t mutexattr;
    };

    struct mt* mm;

    mm = (mt*) mmap(NULL, sizeof(*mm), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    memset(mm ,0x00, sizeof(*mm));
        
    pthread_mutexattr_init(&mm->mutexattr);
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mm->mutex, &mm->mutexattr);

}
