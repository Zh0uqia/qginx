#include <iostream>
#include <Process.h>

pid_t Process::spawnProcess(TCallback proc, void *data, int serverFD, struct mt* shmMutex){
    pid_t pid;

    pid = fork();

    switch(pid){

        case -1:
            perror("Failure of forking a child process: ");
            return pid;

        case 0:
            // if fork() success, start worker process 
            dbPrint( "[son] " << getpid() << " from [parent] " << getppid() << "\n" << std::endl );
            proc(data, serverFD, shmMutex);
            break;
            
        default:
            break;

    }
                
    return pid;
                
}
