#include <iostream>
#include <RequestHandler.h>
#include <regex>

RequestHandler::RequestHandler()
    : state_(STATE_STATUSLINE), 
    method_(METHOD_GET_STATIC),
    httpVersion_(HTTP_10),
    nowReadPos_(0)
{}

RequestHandler::getState(){
    return RequestHandler::state_;
}

StatusState RequestHandler::processStatus(std::string cmd){
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
        uri_ = "test.php"; // "index.html"
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
                queryString_ = statusLine.substr(tmp, nxt-tmp);
            }

            if (uri_ == "/"){
                uri_ = "test.php"; // "index.html"
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

#ifdef DEBUG
    std::cout << "uri is \n" << uri_ << std::endl;
    std::cout << "query string is \n" << queryString_ << std::endl;
    std::cout << "method is \n" << method_ << std::endl;
    std::cout << "status line is \n" << statusLine << std::endl;
    std::cout << "remaining is \n" << cmd << std::endl;
#endif 
    return STATUS_FINISH;
}

HeaderState RequestHandler::processHeader(std::string cmd){
    if (nowReadPos_ == std::string::npos)
        return HEADER_ERROR;

    std::string header = cmd.substr(nowReadPos_);
    std::smatch m;
    std::regex b("[\r][\n][\r][\n]");

    if (std::regex_search(cmd, m, b)){
        std::cout << "empty line found" << std::endl;
    }else{
        return HEADER_ERROR;
    }

#ifdef DEBUG
    for (unsigned i=0; i<m.size(); ++i){
        std::cout << "match:" << m.position(i) << std::endl;
    }
#endif

    nowReadPos_ = m.position(m.size()-1) + 4;

#ifdef DEBUG
    std::cout << "Header is \n" << header << std::endl;
#endif
    
    return HEADER_FINISH;
}

BodyState RequestHandler::processBody(std::string cmd){
    if (nowReadPos_ < cmd.length()){
        body_ = cmd.substr(nowReadPos_);
#ifdef DEBUG 
        std::cout << "body is \n" << cmd.substr(nowReadPos_) << std::endl;
#endif 
        return BODY_FINISH;
    }else{
        return BODY_ERROR;
    }
}

void RequestHandler::processRequest(std::string cmd){
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

#ifdef DEBUG
    std::cout << "state after statusline is: " << state_ << std::endl;
#endif

    if (state_ == STATE_HEADER){
        HeaderState flag = processHeader(cmd);

        if (flag == HEADER_ERROR)
            state_ = STATE_ERROR;
        else
            state_ = STATE_BODY;
    }


#ifdef DEBUG
    std::cout << "state after statusline is: " << state_ << std::endl;
#endif

    if (state_ == STATE_BODY){
        BodyState flag = processBody(cmd);

        if (flag == BODY_ERROR)
            state_ = STATE_ERROR;
        else
            state_ = STATE_FINISH;
    }
    

#ifdef DEBUG
    std::cout << "state after statusline is: " << state_ << std::endl;
#endif

    return;
}

