#include <iostream>
#include <CommParser.h>
#include <cstring>

Request CommParser::parseCommand(std::string cmd){
    // char* token = std::strtok(cmd, " ");
    std::istringstream buf(cmd);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);

    std::string parsed[CommParser::MAXCMDLENGTH];
    int i;
    Request r;

    i=0;
    for (auto& s: tokens){
        parsed[i] = s;
        i++;
    }
    
    for (i=0; i<CommParser::MAXCMDLENGTH; i++){
            std::string seg = parsed[i];

            if (CommParser::isCmd(seg))
                r.setType(seg);
            if (CommParser::isPath(seg))
                r.setPath(seg);
            if (CommParser::isUrl(seg))
                r.setUrl(seg);
    }
    
    return r;
}

bool CommParser::isCmd(std::string s){
    return true;
}

bool CommParser::isPath(std::string s){
    return true;
}

bool CommParser::isUrl(std::string s){
    return true;
}
