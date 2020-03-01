#pragma once
#include <conf.h>

class Request
{
public:
    Request();
    ~Request();

    std::string getType();
    std::string getPath();
    std::string getUrl();
    void setType(std::string t);
    void setPath(std::string p);
    void setUrl(std::string u);

private:
    std::string type;
    std::string file_path;
    std::string url;
};

