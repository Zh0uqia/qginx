#include <iostream>
#include <MasterProcess.h>

void MasterProcess::startMasterProcess(cycle_t* cycle){
    mutexInit();
    startWorkerProcess(cycle);

}

void MasterProcess::startWorkerProcess(cycle_t* cycle){
    int i;
    pid_t pid;

    // for (i=0; i<WORKER_NUMBER; i++){
    for (i=0; i<1; i++){
        Process pcs;
        WorkerProcess workerProcess;

        auto cb = std::bind(&WorkerProcess::workerProcessCycle, \
                            &workerProcess, std::placeholders::_1, \
                            std::placeholders::_2, std::placeholders::_3);

        pid = pcs.spawnProcess(cb, (void *) (intptr_t) i, cycle, mutexShmPTR);
        if (pid == 0)
            break;
        
    }
    
}

void MasterProcess::mutexInit(){
    mm = (mt*) mmap(NULL, sizeof(*mm), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    memset(mm ,0x00, sizeof(*mm));

    pthread_mutexattr_init(&mm->mutexattr);
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mm->mutex, &mm->mutexattr);

    // create shared memory space and bind it with a pointer 
    mutexShmID = shmget(IPC_PRIVATE, sizeof(mt), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    mutexShmPTR = (mt*) shmat(mutexShmID, 0, 0);
    mutexShmPTR = mm;

}
