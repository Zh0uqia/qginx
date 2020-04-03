#include <iostream>
#include <Process.h>

void Process::startMasterProcess(){
   startWorkerProcess(WORKER_NUMBER);

}

void Process::startWorkerProcess(int n){
    int i;

    for (i=0; i<n; i++){
        /*if(fork() == 0){
            dbPrint( "[son]" << getpid() << "from [parent]" << getppid() << "\n" << std::endl);
            exit(0);
        }*/ 
        spawnProcess();
    }
}

void Process::spawnProcess(){
    pid_t pid;

    pid = fork();

    switch(pid){

    case -1:
        perror("Failure of forking a child process: ");
        return;

    case 0:
        // if fork() success, start worker process 
        dbPrint( "[son] " << getpid() << " from [parent] " << getppid() << "\n" << std::endl);
        exit(0);
        break;

    default:
        break;
    }
}
