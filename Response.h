#pragma once
#include <conf.h>
#include <stdlib.h>

class Response
{
public:
    char* generateResponse(char* content);

private:
    std::string status_line = "HTTP/1.0 200 OK\r\n";
    std::string date = "Date: Thu, 19 Feb 2009 12:27:04 GMT\r\n";
    std::string server = "SERVER: QYSERVER\r\n";
    std::string length = "Content-Length: ";
    std::string type = "Content-Type: text/html\r\n";
    std::string charset = "charset=UTF-8\r\n";

};

