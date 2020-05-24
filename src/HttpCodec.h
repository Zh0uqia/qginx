#pragma once
#include <conf.h>
#include <TransportDirection.h>

#include <external/http-parser/http_parser.h>

// stream http message into objects 
class HttpCodec
{
public:
    HttpCodec(TransportDirection direction);
    ~HttpCodec();

    const http_parser_settings* generateSettings();

    void onIngress(char* buf, ssize_t recved);

    /* Implementations of callback functions for parser*/
    void onMessageBegin();
    int onUrl(const char* buf, size_t len);
    int onHeaderField(const char* buf, size_t len);
    int onHeaderValue(const char* buf, size_t len);
    int onHeaderComplete();
    int onBody(const char* buf, size_t len);
    int onMessageComplete();

    /* Callback wrappers for http_parser callbacks*/
    static int onMessageBeginCB(http_parser* parser);
    static int onUrlCB(http_parser* parser, const char * buf, size_t len);
    static int OnHeaderFieldCB(http_parser *parser, const char * buf, size_t len);
    static int OnHeaderValueCB(http_parser *parser, const char * buf, size_t len);
    static int OnHeadersCompleteCB(http_parser *parser);
    static int OnBodyCB(http_parser *parser, const char * buf, size_t len);
    static int OnMessageCompleteCB(http_parser *parser);
                                   
private:
    TransportDirection direction_;

    http_parser* parser_;


};

