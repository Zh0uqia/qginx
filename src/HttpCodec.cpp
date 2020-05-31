#include <iostream>
#include <HttpCodec.h>

HttpCodec::HttpCodec(TransportDirection direction)
    : direction_(direction),
    headerParsingState_(HeaderParsingState::kParsingHeaderIdle)
{
    // remember to free
    parser_ = (http_parser*) malloc(sizeof(http_parser)); 
    switch(direction){
    case TransportDirection::DOWNSTREAM:
        http_parser_init(parser_, HTTP_REQUEST);
        break;
    case TransportDirection::UPSTREAM:
        http_parser_init(parser_, HTTP_RESPONSE);
        break;
    default:
        std::cout << "Unknown direction" << std::endl;
    }

    parser_->data = this;
}

HttpCodec::~HttpCodec(){
    free(parser_);
    parser_ = nullptr;
}

void HttpCodec::setCallback(HttpSession* session){
    callback_ = session;
}

const http_parser_settings* HttpCodec::generateSettings(){
    static http_parser_settings settings;

    settings.on_message_begin = HttpCodec::onMessageBeginCB;
    settings.on_url = HttpCodec::onUrlCB;
    settings.on_header_field = HttpCodec::onHeaderFieldCB;
    settings.on_header_value = HttpCodec::onHeaderValueCB;
    settings.on_headers_complete = HttpCodec::onHeadersCompleteCB;
    settings.on_body = HttpCodec::onBodyCB;
    settings.on_message_complete = HttpCodec::onMessageCompleteCB;

    return &settings;
}

void HttpCodec::onIngress(char* buf, ssize_t recved){
    size_t nparsed;
    dbPrint("Received message length is: " << recved << std::endl);
    dbPrint("Received message is: " << buf << std::endl);
    /* callback functions will be called after http_parser_execute() */
    nparsed = http_parser_execute(parser_, generateSettings(), buf, recved);

}

int HttpCodec::onMessageBegin(){
    msg_.reset(new HttpMessage());
    
    callback_ -> onMessageBegin(msg_.get());
    return 0;
}

int HttpCodec::onUrl(const char* buf, size_t len){
    url_.append(buf, len);
    return 0;
}

/*
void HttpCodec::pushHeaderNameAndValue(HttpHeaders& hdrs){
    hdrs.add(currentHeaderName_, currentHeaderValue_);
    currentHeaderName_.clear();
    currentHeaderValue_.clear();
    // currentHeaderNameString_.clear();
}
*/

int HttpCodec::onHeaderField(const char* buf, size_t len){
    dbPrint("Current header field: " << buf << std::endl);

    /*
    if (headerParsingState_ == kParsingHeaderValue){
        pushHeaderNameAndValue(msg_.getHeaders());
    }else if (headerParsingState_ == kParsingHeaderName){
        currentHeaderName_.append(buf, len)
    }else{
        headerParsingState_ = kParsingHeaderName;
        currentHeaderName_.append()
    }
    // currentHeaderNameString_.append(buf);
    */

    return 0;
}

int HttpCodec::onHeaderValue(const char* buf, size_t len){
    dbPrint("Current header value: " << buf << std::endl);
    
    // currentHeaderValue_.append(buf, len);
    return 0;
}

int HttpCodec::onHeadersComplete(){
    msg_->setHttpVersion(parser_->http_major, parser_->http_minor);

    if (direction_ == TransportDirection::DOWNSTREAM){
        msg_->setHttpMethod(static_cast<http_method>(parser_->method));
    }
    
    msg_->setUrl(url_);
    url_.clear();

    headerParsingState_ = HeaderParsingState::kParsingHeaderComplete;

    callback_->onHeadersComplete(msg_.get());

    return 0;
}

int HttpCodec::onBody(const char* buf, size_t len){return 0;}
int HttpCodec::onMessageComplete(){return 0;}

/* All callback functions must return 0 on success, return 1 on failure */
int HttpCodec::onMessageBeginCB(http_parser* parser){
    HttpCodec* codec = static_cast<HttpCodec*>(parser->data);

    try{
        return codec->onMessageBegin();
    }catch(const std::exception& ex){
        return 1;
    }
}

int HttpCodec::onUrlCB(http_parser* parser, const char* buf, size_t len){
    // static member function does not have "this" pointer
    // and cannot access non-static member function directly
    HttpCodec* codec = static_cast<HttpCodec*>(parser->data);
    
    try{
        return codec->onUrl(buf, len);
    }catch(const std::exception& ex){
        dbPrint("url wrong" << std::endl);
        return 1;
    }
}

int HttpCodec::onHeaderFieldCB(http_parser *parser, const char * buf, size_t len){
    HttpCodec* codec = static_cast<HttpCodec*>(parser->data);

    try{
        return codec->onHeaderField(buf, len);
    }catch(const std::exception& ex){
        dbPrint("header field wrong" << std::endl);
        return 1;
    }
}

int HttpCodec::onHeaderValueCB(http_parser *parser, const char * buf, size_t len){
    HttpCodec* codec = static_cast<HttpCodec*>(parser->data);

    try{
        return codec->onHeaderValue(buf, len);
    }catch(const std::exception& ex){
        dbPrint("header value wrong" << std::endl);
        return 1;
    }

}

int HttpCodec::onHeadersCompleteCB(http_parser *parser){
    HttpCodec* codec = static_cast<HttpCodec*>(parser->data);

    try{
        return codec->onHeadersComplete();
    }catch(const std::exception& ex){
        dbPrint("header complete wrong" << std::endl);
        return 1;
    }

}

int HttpCodec::onBodyCB(http_parser *parser, const char * buf, size_t len){return 0;}
int HttpCodec::onMessageCompleteCB(http_parser *parser){return 0;}
     
