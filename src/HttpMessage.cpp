#include <HttpMessage.h>

void HttpMessage::setHttpVersion(uint8_t maj, uint8_t min){
   versionStr_.append(1, maj+'0');
   versionStr_.append(1, '.');
   versionStr_.append(1, min+'0');
   dbPrint("http version is " << versionStr_ << std::endl);
}

void HttpMessage::setHttpMethod(enum http_method m){
    const char* temp = method_strings[m];
    std::string s(temp);
    method_ = s;

    dbPrint("method is " << method_ << std::endl);
}

void HttpMessage::setUrl(std::string url){
    url_ = url;

    dbPrint("url is " << url_ << std::endl);
}

void HttpMessage::setHandler(Handler handler){

}

std::string HttpMessage::getHttpVersion(){
    return versionStr_;
}

std::string HttpMessage::getHttpMethod(){
    return method_;
}

std::string HttpMessage::getUrl(){
    return url_;
}
