#include <iostream>
#include <Server.h>

void Server::start(int p){
    std::cout << "started" << std::endl;
    started_ = true;
    port_ = p; 
}

void Server::close(){
    started_ = false;
}

void Server::handNewConn(){
    // int server_fd, new_socket, valread;
    // struct sockaddr_in address;
    // int opt = 1;
    // int addrlen = sizeof(address);
    
    // char buffer[1024];
    // const char *hello = "Hello from server";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::perror("socket failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket created" << std::endl;
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        std::perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket opt set" << std::endl;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        std::perror("bind failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket binded" << std::endl;
    if (listen(server_fd, 3) < 0)
    {
        std::perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket listening" << std::endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        std::cout << "cannot accept" << std::endl;
        std::perror("accept");
        exit(EXIT_FAILURE);
    }


    valread = read( new_socket , buffer, 1024); 
    std::cout << "valread = "<< valread << std::endl; 
    send(new_socket , hello , strlen(hello) , 0 ); 
    std::cout << "Hello message sent" << std::endl;
    printf("%s", buffer);    
    
    
    CommParser cp;
    
    Request rq;

    std::string cmd(buffer);
     
    rq = cp.parseCommand(cmd);
    printf("request generated;");
    
    Dispatcher dsp;
    dsp.dispatch(rq);

    printf("dispatcher generated");
     
}
