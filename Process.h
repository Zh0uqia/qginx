#pragma once
#include <conf.h>
#include <stdio.h>
#include <unistd.h>

class Process
{
public:
    typedef struct {
        int pid;
    } process_t;

    void startMasterProcess();
    
    void startSingleProcess();

    void startWorkerProcess(int n);
    
    void spawnProcess();

private:
    
};

