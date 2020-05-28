#pragma once
#include <Handler.h>
#include <HttpMessage.h>

class SimpleController
{
public:
    Handler* getRequestHandler(HttpMessage* msg);

private:
};

