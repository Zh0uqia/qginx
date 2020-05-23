#include <iostream>
#include <HttpCodec.h>

HttpCodec::HttpCodec(TransportDirection direction)
    : direction_(direction)
{
    parser_ =(http_parser*) malloc(sizeof(http_parser)); 
    switch(direction){
    case TransportDirection::DOWNSTREAM:
        http_parser_init(parser_, HTTP_REQUEST);
        break;
    case TransportDirection::UPSTREAM:
        http_parser_init(parser_, HTTP_RESPONSE);
        break;
    default:
        std::cout << "Unknown direction" << std::endl;
    }

    parser_->data = this;
}

const http_parser_settings* HttpCodec::generateSettings(){
    http_parser_settings settings;

    settings.on_message_begin = HttpCodec::onMessageBeginCB;
    settings.on_url = HttpCodec::onUrlCB;

    return &settings;
}

void HttpCodec::onIngress(char* buf, ssize_t recved){
    size_t nparsed;

    nparsed = http_parser_execute(parser_, generateSettings(), buf, recved);
    dbPrint(nparsed << std::endl);
}

void HttpCodec::onMessageBegin(){}
void HttpCodec::onUrl(){}

int HttpCodec::onMessageBeginCB(http_parser* parser){}
int HttpCodec::onUrlCB(http_parser* parser, const char* buf, size_t len){}


