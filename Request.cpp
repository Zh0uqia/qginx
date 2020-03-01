#include <iostream>
#include <Request.h>

std::string Request::getType(){
    return Request::type; 
}

std::string Request::getUrl(){
    return Request::url;
}

std::string Request::getPath(){
    return Request::file_path;
}

void Request::setType(std::string t){
    Request::type = t;
}

void Request::setPath(std::string p){
    Request::file_path = p;
}

void Request::setUrl(std::string u){
    Request::url = u;
}
