#include <iostream>
#include <HttpMessage.h>
#include <unistd.h>

class HttpMessage;
class ResponseBuilder{
public:
    ResponseBuilder& status(uint16_t code, const std::string& message){
        header_ = std::make_unique<HttpMessage>();
        header_ -> setHttpVersion(1, 1);
        header_ -> setStatusCode(code);
        header_ -> setStatusMessage(message);
        return *this;

    }

    ResponseBuilder& body(std::string bodyIn){
        body_ = bodyIn;
        return *this;

    }

    void send(int fd){
        std::string response = header_->generateResponse();
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(body_.size()) +
            "\r\n";
        response += "Server: Qginx\r\n";
        response += "\r\n";
        response += body_;
        const char* writeBuf = response.c_str();

        if (write(fd, writeBuf, response.size()) < 0){
            std::perror("Write to socket");
        }
    
    }

private:
    int socketFd_;
    std::unique_ptr<HttpMessage> header_;
    std::string body_;
};
