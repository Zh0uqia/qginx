#pragma once
#include <conf.h>
#include <functional>
#include <MasterProcess.h>
#include <Mutex.h>
#include <Core.h>

class Process
{
public:
    using TCallback = std::function<void(void *data, cycle_t* cycle, struct mt* shmMutex)>;
    
    pid_t spawnProcess(TCallback proc, void *data, cycle_t* cycle, struct mt* shmMutex);

private:
    
};

