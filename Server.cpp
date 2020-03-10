#include <iostream>
#include <Server.h>
#include <regex>

void Server::start(int p){
    std::cout << "started" << std::endl;
    started_ = true;
    port_ = p; 
}

void Server::close(){
    started_ = false;
}

void Server::handNewConn(){
    // Creating socket file descriptor
    int opt = 1;

    if ((serverFD_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::perror("socket failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket created" << std::endl;
    // Forcefully attaching socket to the port 8080
    if (setsockopt(serverFD_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        std::perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket opt set" << std::endl;
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(port_);

    // Forcefully attaching socket to the port 8080
    if (bind(serverFD_, (struct sockaddr *)&address_,
                                 sizeof(address_))<0)
    {
        std::perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket binded" << std::endl;
    if (listen(serverFD_, 3) < 0)
    {
        std::perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket listening" << std::endl;

    while (true){
        int addrlen = sizeof(address_);

        if ((newSocket_ = accept(serverFD_, (struct sockaddr *)&address_,
                           (socklen_t*)&addrlen))<0)
        {
            std::cout << "cannot accept" << std::endl;
            std::perror("accept");
            exit(EXIT_FAILURE);
        }

        char* hello = "Hello from server";
        memset(readBuffer_, 0, sizeof(readBuffer_));

        int valread = read( newSocket_ , readBuffer_, 1024); 
        std::cout << "valread = "<< valread << std::endl; 
        send(newSocket_ , hello , strlen(hello) , 0 ); 
        std::cout << "Hello message sent" << std::endl;
        std::cout << readBuffer_ << std::endl;    
        
#ifdef DEBUG
        std::string cmd(readBuffer_);
        
        std::regex b("[\r][\n][\r][\n]");

        if (std::regex_search(cmd, b))
            std::cout << "matched" << std::endl;
        else
            std::cout << "not matched" << std::endl;
        
#endif 
        RequestHandler requestHandler;
        requestHandler.processRequest(cmd);
                
        /*
        if (ps == STATE_PRS_FINISH){
            Dispatcher dsp;
            char* response;
            response = dsp.dispatch(requestHandler);

            send(newSocket_, response, strlen(response), 0);
        }else{
            char* error = "Error from Server";
            send(newSocket_, error, strlen(error), 0);
        }
        */ 

        /*

        Dispatcher dsp;
        char* response;
        response = dsp.dispatch(rq);

        send(new_socket, response, strlen(response), 0);
        */
    }
}
