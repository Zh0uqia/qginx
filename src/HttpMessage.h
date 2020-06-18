#pragma once
#include <conf.h>
#include <external/http-parser/http_parser.h>
#include <utils/ParseURL.h>

// class Handler;
class HttpMessage
{
public:
    HttpMessage();

    void setHttpVersion(uint8_t mag, uint8_t min);
    void setHttpMethod(enum http_method m);
    void setUrl(std::string url);
    void setStatusCode(uint16_t code);
    void setStatusMessage(const std::string& message);
    // void setHandler(Handler* handler);

    std::string generateResponse();
 
    std::string getHttpVersion();
    std::string getHttpMethod();
    std::string getUrl();
    std::string getFilePath();
 
private:
    // Handler* handler_;

    std::string versionStr_;
    std::string url_;
    std::string method_;

    uint16_t code_;
    std::string statusMessage_;
};

