#include <iostream>
#include <Response.h>
#include <bits/stdc++.h>

char* Response::generateResponse(char* content){
    // convert content to string
    /*
    std::string res;

    
    std::string res = status_line + date + server +\
          length + std::to_string(sizeof(content)) + "\r\n" +\
          type + "\r\n" + content;
    
    std::string res = "HTTP/1.0 200 OK\r\nContent-Length: 11\r\nContent-Type: text/html; charset=UTF-8\r\n\r\nHello World\r\n";
    std::cout << res << std::endl;
    

    char* response = (char*) malloc(10000 * sizeof(char));
    strcpy(response, res.c_str());

    return response;
    */ 
    return content;
}

