#include <HttpMessage.h>

static const char *method_strings[] =
          { "DELETE"
            , "GET"
            , "HEAD"
            , "POST"
            , "PUT"
            , "CONNECT"
            , "OPTIONS"
            , "TRACE"
            , "COPY"
            , "LOCK"
            , "MKCOL"
            , "MOVE"
            , "PROPFIND"
            , "PROPPATCH"
            , "UNLOCK"
            , "REPORT"
            , "MKACTIVITY"
            , "CHECKOUT"
            , "MERGE"
            , "M-SEARCH"
            , "NOTIFY"
            , "SUBSCRIBE"
            , "UNSUBSCRIBE"
            , "PATCH"
            };


HttpMessage::HttpMessage(){}

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

void HttpMessage::setStatusCode(uint16_t code){
    code_ = code;
}

void HttpMessage::setStatusMessage(const std::string& message){
    statusMessage_ = message;
}
 
void HttpMessage::setUrl(std::string url){
    url_ = url;

    dbPrint("url is " << url_ << std::endl);
}

/*
void HttpMessage::setHandler(Handler* handler){
    handler_ = handler;
}
*/

std::string HttpMessage::generateResponse(){
    std::string res;

    dbPrint("Code is: " << code_ << std::endl);
    res += "HTTP/" + versionStr_ + " " + std::to_string(code_) +
        " " + statusMessage_ + "\r\n";
    return res;
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
 
std::string HttpMessage::getFilePath(){
    ParseURL u(url_);
    std::string path = u.path();
    if (path == "/")
        path.append("index.html");
    std::string fpath = '.' + path; // get relative path

    return fpath;
}

 
