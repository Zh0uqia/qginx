#pragma once
#include <string>
#include <conf.h>
#include <Request.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

class CommParser
{
public:
    CommParser();
    ~CommParser();
    Request parseCommand(std::string cmd);
    bool isCmd(std::string s);
    bool isPath(std::string s);
    bool isUrl(std::string s);
private:
    static const int MAXCMDLENGTH = 20;
};

