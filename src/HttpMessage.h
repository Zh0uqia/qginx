#pragma once
#include <conf.h>
#include <Handler.h>
#include <external/http-parser/http_parser.h>

static const char *method_strings[] =
          { "DELETE"
            , "GET"
            , "HEAD"
            , "POST"
            , "PUT"
            , "CONNECT"
            , "OPTIONS"
            , "TRACE"
            , "COPY"
            , "LOCK"
            , "MKCOL"
            , "MOVE"
            , "PROPFIND"
            , "PROPPATCH"
            , "UNLOCK"
            , "REPORT"
            , "MKACTIVITY"
            , "CHECKOUT"
            , "MERGE"
            , "M-SEARCH"
            , "NOTIFY"
            , "SUBSCRIBE"
            , "UNSUBSCRIBE"
            , "PATCH"
            };

class HttpMessage
{
public:
    void setHttpVersion(uint8_t mag, uint8_t min);
    void setHttpMethod(enum http_method m);
    void setUrl(std::string url);
    void setHandler(Handler handler);

    std::string getHttpVersion();
    std::string getHttpMethod();
    std::string getUrl();

private:
    std::string versionStr_;
    std::string url_;
    std::string method_;
};

