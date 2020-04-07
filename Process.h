#pragma once
#include <conf.h>
#include <functional>
#include <MasterProcess.h>
#include <Mutex.h>

class Process
{
public:
    using TCallback = std::function<void(void *data, int serverFD, struct mt* shmMutex)>;
    
    pid_t spawnProcess(TCallback proc, void *data, int serverFD, struct mt* shmMutex);

private:
    
};

