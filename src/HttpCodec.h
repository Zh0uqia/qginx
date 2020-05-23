#pragma once
#include <conf.h>
#include <TransportDirection.h>

#include <external/http-parser/http_parser.h>

// stream http message into objects 
class HttpCodec
{
public:
    HttpCodec(TransportDirection direction);
    
    const http_parser_settings* generateSettings();

    void onIngress(char* buf, ssize_t recved);

    /* Implementations of callback functions for parser*/
    void onMessageBegin();
    void onUrl();
    
    /* Callback wrappers for http_parser callbacks*/
    static int onMessageBeginCB(http_parser* parser);
    static int onUrlCB(http_parser* parser, const char * buf, size_t len);

private:
    TransportDirection direction_;

    http_parser* parser_;


};

