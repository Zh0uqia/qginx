#pragma once
#include <Controller.h>
#include <conf.h>
#include <Request.h>

class Dispatcher
{
public:
    Dispatcher();
    ~Dispatcher();
    
    void dispatch(Request r);
private:

};

