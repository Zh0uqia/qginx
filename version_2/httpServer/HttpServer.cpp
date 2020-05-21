#include <iostream>

#include <httpServer/HttpServer.h>

// Acceptor factory that create new http server acceptor 
class AcceptorFactory{
    public:
        AcceptorFactory(HttpServerOptions options,
                        HttpCodecFactory codecFactory
                        // HttpSession::InfoCallback* sessionInfoCb
                        ):
            options_(options),
            codecFactory_(codecFactory)
            // sessionInfoCb_(sessionInfoCb) 
            {}
        HttpServerAcceptor newAcceptor(){}

    private:
        HttpServerOptions options_;
        HttpCodecFactory codecFactory_;
        // HttpSession::InfoCallback* sessionInfoCb_;
};

HttpServer::HttpServer(HttpServerOptions options) :
    started_(false),
    options_(options) {}

void HttpServer::bindPort(IPConfig const& ips){
    address_ = ips;
}

void HttpServer::startTcpServer(){
    // Creating socket file descriptor
    int opt = 1;
    int port = address_.port;
    
    if ((serverFd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::perror("Socket failed");
    }
    
    if (setsockopt(serverFd_, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)) <0 )
    {
        std::perror("set sockopt failed");
    }

    memset(&socketAddress_, '\0', sizeof(socketAddress_));
    socketAddress_.sin_family = AF_INET;
    socketAddress_.sin_addr.s_addr = INADDR_ANY;
    socketAddress_.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(serverFd_, (struct sockaddr *)&socketAddress_,
             sizeof(address_))<0)
    {
        std::perror("bind failed");
    }

    if (listen(serverFd_, options_.listenBacklog) < 0)
    {
        std::perror("listen failed");
    }

    /*
    if (setNonBlocking() == -1){
        std::perror("set non-blocking failed");
        close(serverFD);
        exit(EXIT_FAILURE);
    }
    */

    std::cout << "server is started" << std::endl;
    started_ = true;       
}

void HttpServer::start(std::function<void()> onSuccess,
                       std::function<void(std::exception_ptr)> onError){
    // masterProcess_ = new MasterProcess();
    startTcpServer();
    
    if (!started_)
        stop();
    // else
        // MasterProcess masterProcessCycle();
}

void HttpServer::stopListening(){
    // terminate master process cycle 
}

void HttpServer::stop(){
    // stopListening();
    
    /*
    if (signalHandler_){
        signalHandler_.reset();
    }
    */ 
}

int HttpServer::getListenSocket() const{
    return serverFd_;
}
