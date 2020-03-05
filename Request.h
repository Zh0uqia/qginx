#pragma once
#include <conf.h>

class Request
{
public:
    std::string getMethod();
    std::string getPath();
    std::string getHttpVersion();
    std::string getQueryString();
    std::string getContent();
    void setMethod(std::string m);
    void setPath(std::string p);
    void setHttpVersion(std::string v);
    void setQueryString(std::string q);
    void setContent(std::string c);

private:
    std::string method;
    std::string file_path;
    std::string http_version;
    
    std::string query_string;
    std::string content;
};

