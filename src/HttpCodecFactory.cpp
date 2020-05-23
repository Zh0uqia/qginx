#include <iostream>
#include <HttpCodecFactory.h>

std::unique_ptr<HttpCodec> HttpCodecFactory::getCodec(TransportDirection direction){
    return std::make_unique<HttpCodec>(direction);
}
