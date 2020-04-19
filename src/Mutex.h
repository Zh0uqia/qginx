#pragma once
#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

struct mt{
    int num;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;

};
    

