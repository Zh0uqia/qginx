#pragma once
#include <conf.h>
#include <HttpCodec.h>
#include <TransportDirection.h>

// create new codec 
class HttpCodecFactory
{
public:
    std::unique_ptr<HttpCodec> getCodec(TransportDirection direction);    

private:

};

