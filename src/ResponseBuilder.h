#include <iostream>
#include <HttpMessage.h>

class ResponseBuilder{
    public:
        ResponseBuilder& status(uint8_t code, const std::string& message){
            header_ = std::make_unique<HttpMessage>();
            header_ -> setHttpVersion(1, 1);
            header_ -> setStatusCode(code);
            header_ -> setStatusMessage(message);
            return *this;
        }

        ResponseBuilder& body(std::string bodyIn){
            body_ = std::move(bodyIn);
            return *this;
        }

        void send(){
            if (header_){
                std::string response = header_->generateResponse();
                response += "Content-Length: " + std::to_string(body_.size()) +
                    "\r\n\r\n";
                const char* writeBuf = response.c_str();

                if (write(fd, writeBuf, response.size()) < 0){
                    std::perror("Write to socket");
                }
            }

            if (body_){
                if (write(fd, body_.c_str(), body_.size()) < 0){
                    std::perror("Write to socket");
                }
            }
        }
    private:
        int socketFd_;
        std::unique_ptr<HttpMessage> header_;
        std::unique_ptr<HttpMessage> body_;
};
