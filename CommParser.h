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
    Request parseCommand(std::string cmd);
private: 
    std::vector<std::string> split_string(std::string&, std::string&);
    void print_string(std::vector<std::string>&);
    bool hasQueryString(std::string);
};
