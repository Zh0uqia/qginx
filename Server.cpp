#include <iostream>
#include <Server.h>
#include <regex>

void Server::start(int p){
    dbPrint("started" << std::endl);
    started_ = true;
    port_ = p; 
}

void Server::handleSigpipe(int signum){
    dbPrint("Caught signal SIGPIPE %d\n", signum);
}

void Server::handNewConn(){
    // Creating socket file descriptor
    int opt = 1;

    if ((serverFD_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::perror("socket failed");
        exit(EXIT_FAILURE);
    }
    dbPrint("socket created" << std::endl);
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(serverFD_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        std::perror("set sockopt failed");
        exit(EXIT_FAILURE);
    }
    dbPrint("socket opt set" << std::endl);

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
    dbPrint("socket binded" << std::endl);

    if (listen(serverFD_, 3) < 0)
    {
        std::perror("listen failed");
        exit(EXIT_FAILURE);
    }
    dbPrint("socket listening" << std::endl);
    
    while (true){
        signal(SIGPIPE, handleSigpipe);

        int addrlen = sizeof(address_);

        if ((newSocket_ = accept(serverFD_, (struct sockaddr *)&address_,
                           (socklen_t*)&addrlen))<0)
        {
            std::perror("accept failer");
            exit(EXIT_FAILURE);
        }

        memset(readBuffer_, 0, sizeof(readBuffer_));

        int valread = read( newSocket_ , readBuffer_, 1024); 
        dbPrint("valread = "<< valread << std::endl); 
        dbPrint(readBuffer_ << std::endl);
        
        std::string cmd(readBuffer_);

        RequestHandler requestHandler;
        requestHandler.processRequest(cmd);
    
        ProcessState ps = requestHandler.getState();
        
        if (ps == STATE_FINISH){
            dbPrint("-----------STATE FINISH------------" << std::endl);
            Dispatcher dsp;
            char* response;
            response = dsp.dispatch(requestHandler);

            char send_buff[10000];
            std::string header_buff;
            header_buff = "HTTP/1.1 " + std::to_string(220) + " OK" + "\r\n";
            header_buff += "Content-Type: text/html\r\n";
            header_buff += "Connection: Close\r\n";
            header_buff += "Content-Length: " + std::to_string(strlen(response)) + "\r\n";
            header_buff += "Server: Qianying Zhou's Web Server\r\n";
            header_buff += "\r\n";
            
            sprintf(send_buff, "%s", header_buff.c_str());

            write(newSocket_, send_buff, strlen(send_buff));

            write(newSocket_, response, strlen(response));
        }else{
            dbPrint("-----------STATE ERROR ------------" << std::endl);
        }

        close(newSocket_);         
    }
}
