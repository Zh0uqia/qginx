#include <iostream>
#include <Request.h>

std::string Request::getMethod(){
    return Request::method; 
}

std::string Request::getHttpVersion(){
    return Request::http_version;
}

std::string Request::getPath(){
    return Request::file_path;
}

std::string Request::getQueryString(){
    return Request::query_string;
}

std::string Request::getContent(){
    return Request::content;
}

void Request::setMethod(std::string m){
    Request::method = m;
}

void Request::setPath(std::string p){
    Request::file_path = p;
}

void Request::setHttpVersion(std::string v){
    Request::http_version = v;
}

void Request::setQueryString(std::string q){
    Request::query_string = q;
}

void Request::setContent(std::string c){
    Request::content = c;
}
