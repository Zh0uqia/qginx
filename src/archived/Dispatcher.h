#pragma once
#include <archived/Controller.h>
#include <conf.h>
#include <archived/RequestHandler.h>

class Dispatcher
{
public:
    std::string dispatch(RequestHandler rhandl);
private:

};

