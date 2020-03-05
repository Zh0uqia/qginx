#pragma once
#include <conf.h>
#include <stdlib.h>

class Response
{
public:
    char* generateResponse(char* content);

private:
    std::string status_line = "HTTP/1.0 200 OK";
    std::string server = "SERVER: QYSERVER";
    std::string length = "Content-Length: ";
    std::string type = "Content-Type: text/html";
    std::string charset = "charset=UTF-8\r\n";

};

