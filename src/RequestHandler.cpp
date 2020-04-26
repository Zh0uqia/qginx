#include <iostream>
#include <RequestHandler.h>
#include <regex>

RequestHandler::RequestHandler()
    : state_(STATE_STATUSLINE), 
    method_(METHOD_GET_STATIC),
    httpVersion_(HTTP_10),
    nowReadPos_(0)
{}

void RequestHandler::reset(){
    state_ = STATE_STATUSLINE;
    method_ = METHOD_GET_STATIC;
    httpVersion_ = HTTP_10;
    nowReadPos_ = 0;
}

ProcessState RequestHandler::getState(){
    return RequestHandler::state_;
}

MethodState RequestHandler::getMethod(){
    return RequestHandler::method_;
}

HttpVersionState RequestHandler::getVersion(){
    return RequestHandler::httpVersion_;
}

std::string RequestHandler::getUri(){
    return RequestHandler::uri_;
}

std::string RequestHandler::getQueryString(){
    return RequestHandler::queryString_;
}

std::string RequestHandler::getBody(){
    return RequestHandler::body_;
}

/*
void RequestHandler::httpWaitRequestHandler(cycle_t *cycle, event_t *ev, int epollFD){
    connection_t *c;
    int n;
    char readBuffer[BUFFERLENGTH];

    memset(readBuffer, 0, sizeof(readBuffer));
    c = (connection_t*) ev->data;
    n = read(c->fd, readBuffer, BUFFERLENGTH); // read request from the socket 
    std::string cmd(readBuffer);

    processRequest(cmd);
    if (state_ == STATE_FINISH){
        Dispatcher dsp;
        char* response;
        response = dsp.dispatch(requestHandler);
        // dbPrint(response << std::endl);

        char send_buff[10000];
        std::string header_buff;
        header_buff = "HTTP/1.1 " + std::to_string(220) + " OK" + "\r\n";
        header_buff += "Content-Type: text/html\r\n";
        header_buff += "Connection: Close\r\n";
        header_buff += "Content-Length: " + std::to_string(strlen(response)) + "\r\n";
        header_buff += "Server: Qianying Zhou's Web Server\r\n";
        header_buff += "\r\n";

        sprintf(send_buff, "%s", header_buff.c_str());
        
        write(c->fd, send_buff, strlen(send_buff));

        write(c->fd, response, strlen(response));
    }else{
        dbPrint("-----------STATE ERROR ------------" << std::endl);                    
    }
}
*/ 

StatusState RequestHandler::processStatus(std::string &cmd){
    size_t pos = nowReadPos_;
    size_t nxt;

    nxt = cmd.find("\r\n", pos);

    if (nxt == std::string::npos)
        return STATUS_ERROR;

    nowReadPos_ = nxt+2;
    std::string statusLine = cmd.substr(pos, nxt-pos);

    // process status line 
    int posGet = statusLine.find("GET");
    int posPost = statusLine.find("POST");
    int posOther = -1;

    for (const auto& elem: methodSet){
        if ((posOther = statusLine.find(elem)) != -1)
            break;
    }
    
    if (posGet >= 0) {
        pos = posGet;
        method_ = METHOD_GET_STATIC;
    } else if (posPost >= 0) {
        pos = posPost;
        method_ = METHOD_POST;
    } else if (posOther >= 0) {
        pos = posOther;
        method_ = METHOD_OTHER;
    } else {
        return STATUS_ERROR;
    }

    // find uri 
    pos = statusLine.find("/", pos);

    if (pos == std::string::npos){
        uri_ = "index.html"; // "index.html"
        return STATUS_FINISH;
    }else{
        nxt = statusLine.find(" ", pos);
        if (nxt == std::string::npos){
            return STATUS_ERROR; 
        }else{
            uri_ = statusLine.substr(pos, nxt-pos);
            size_t tmp;
            if ((tmp = statusLine.find("?", pos)) != std::string::npos){
                method_ = METHOD_GET_DYNAMIC;
                uri_ = statusLine.substr(pos, tmp-pos);
                queryString_ = statusLine.substr(tmp+1, nxt-tmp);
            }

            if (uri_ == "/"){
                uri_ = "index.html"; // "index.html"
            }
        }
    }

    // find http version
    pos = statusLine.find("/", nxt);
    
    if (pos == std::string::npos){
        return STATUS_ERROR;
    }

    if (statusLine.length() - pos <= 3){
        return STATUS_ERROR;
    }else if (statusLine.substr(pos+1) == "1.1"){
        httpVersion_ = HTTP_11;
    }else{
        httpVersion_ = HTTP_10;
    }
/*
    dbPrint("uri is \n" << uri_ << std::endl);
    dbPrint("query string is \n" << queryString_ << std::endl);
    dbPrint("method is \n" << method_ << std::endl);
    dbPrint("status line is \n" << statusLine << std::endl);
*/ 
    return STATUS_FINISH;
}

HeaderState RequestHandler::processHeader(std::string& cmd){
    if (nowReadPos_ == std::string::npos)
        return HEADER_ERROR;

    std::string header = cmd.substr(nowReadPos_);
    std::smatch m;
    std::regex b("[\r][\n][\r][\n]");

    // all match results are stored in array m
    if (std::regex_search(cmd, m, b)){
        // dbPrint("empty line found" << std::endl);
    }else{
        return HEADER_ERROR;
    }

    // position of first char of "\r\n\r\n" + 4
    nowReadPos_ = m.position(m.size()-1) + 4;

    // dbPrint("Header is \n" << header << std::endl);

    return HEADER_FINISH;
}

BodyState RequestHandler::processBody(std::string& cmd){
    if (nowReadPos_ < cmd.length()){
        // should add body if processing POST requests 
        body_ = ""; // we suppose that all get request does not have body 

        // this request ends, read next request 
        cmd = cmd.substr(nowReadPos_);

        // dbPrint("body is \n" << cmd.substr(nowReadPos_) << std::endl); 
    }else{
        // cmd just include one request 
        body_ = "";        
    }
    // get rid of processed requests 
    cmd = cmd.substr(nowReadPos_);

    return BODY_FINISH;
}

void RequestHandler::processRequest(std::string &cmd){
    if (cmd == "" || cmd.empty()){
        state_ = STATE_ERROR;
        return;
    }

    
    if (state_ == STATE_STATUSLINE){
        StatusState flag = processStatus(cmd);

        if (flag == STATUS_ERROR)
            state_ = STATE_ERROR;
        else
            state_ = STATE_HEADER;
    }

    // dbPrint("state after statusline is: " << state_ << std::endl);


    if (state_ == STATE_HEADER){
        HeaderState flag = processHeader(cmd);

        if (flag == HEADER_ERROR)
            state_ = STATE_ERROR;
        else
            state_ = STATE_BODY;
    }

    // dbPrint("state after header is: " << state_ << std::endl);

    if (state_ == STATE_BODY){
        BodyState flag = processBody(cmd);

        if (flag == BODY_ERROR)
            state_ = STATE_ERROR;
        else
            state_ = STATE_FINISH;
    }
    
    // dbPrint("state after body is: " << state_ << std::endl);

    return;
}

