#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include <regex>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/epoll.h>

using namespace std;

int main(){
    int result;
    
    struct epoll_event ev;
    struct mt
    {
        int num;
        pthread_mutex_t mutex;
        pthread_mutexattr_t mutexattr;            
    };

    struct mt* mm;

    mm = (mt*) mmap(NULL, sizeof(*mm), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    memset(mm, 0x00, sizeof(*mm));
    pthread_mutexattr_init(&mm->mutexattr);
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mm->mutex, &mm->mutexattr);

    for (int i=0; i<3; i++){
        result=fork();
        if (result==0){
            
            for (int j=0; j<30; j++){
                pthread_mutex_lock(&mm->mutex);
                printf("worker %d get the mutex \n", i);
                pthread_mutex_unlock(&mm->mutex);
            }

            printf("start worker %d\n", i);
            // break;
            exit(0);
        }
    }
    return 0;
}


