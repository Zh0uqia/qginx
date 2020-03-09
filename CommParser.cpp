#include <iostream>
#include <CommParser.h>
#include <cstring>
#include <algorithm>
#include <vector>


std::vector<std::string> CommParser::split_string(std::string& str,
                                      std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

void CommParser::print_string(std::vector<std::string>& string_vector){
    for (auto i=string_vector.begin(); i!=string_vector.end(); ++i){
        std::cout << *i <<std::endl;
    }
}

bool CommParser::hasQueryString(std::string s){
    if (s.find("?")!=std::string::npos)
        return true;
    else
        return false;

}

Request CommParser::parseCommand(std::string cmd){
    std::string delim = "\r\n";
    std::vector<std::string> parsed = split_string(cmd, delim);

    // print_string(parsed);

    std::vector<std::string>::iterator it, nx, p;

    std::vector<std::string> header, message;
    
    std::string status_line = parsed[0];
    // std::cout << status_line << std::endl;

    int n = parsed.size();
    int chunk = 1;

    it = parsed.begin();
    nx = std::next(it, 1); // skip the status line 

    /*
    while (find(nx, parsed.end(), "\r\n") != parsed.end()){
        p = find(nx, parsed.end(), "\r\n");
        int index = std::distance(it, p);
        std::cout << index << std::endl;

        if (chunk==1){
            std::vector<std::string> hd(nx, p);
            header = hd;
            chunk++;
            nx=p;
            printf("chunk2");
        }
        else if (chunk==2){
            std::vector<std::string> ms(nx, p);
            message = ms;
            chunk++;
            nx=p;
            printf("chunk3");
        }
    }
    //std::cout << status_line << std::endl;
    //std::cout << header << std::endl;
    //std::cout << message << std::endl;

    */
    Request r;
    
    std::string sl_delim = " ";

    // if status line is empty 
    if (status_line == "" || status_line.empty()){
        return r;
    }

    std::vector<std::string> parsed_status = split_string(status_line, sl_delim);
    // print_string(parsed_status);

    if (!hasQueryString(parsed_status[1])){
        r.setPath(parsed_status[1]);
    }
    else{
        int pos = parsed_status[1].find("?");
        std::string query = parsed_status[1].substr(pos+1);
        r.setQueryString(query);
        r.setPath(parsed_status[1].substr(0, pos));
    }

    r.setMethod(parsed_status[0]);
    r.setHttpVersion(parsed_status[2]);
    
    // post request 
    if (r.getMethod() == "POST" && n==8){
        r.setContent(parsed[7]);
    }
    
    return r;
}
