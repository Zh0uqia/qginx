#include <iostream>
#include <Response.h>
#include <bits/stdc++.h>

char* Response::generateResponse(char* content){
    std::string res = status_line + "\r\n" + server + "\r\n" +\
          length + std::to_string(sizeof(content)) + "\r\n" +\
          type + "\r\n" + charset + "\r\n" + content + "\r\n";
    std::cout << res << std::endl;
    char* response = (char*) malloc(10000 * sizeof(char));
    strcpy(response, res.c_str());

    return response;
}

