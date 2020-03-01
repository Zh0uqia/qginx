#include <iostream>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#define IPADDRESS "127.0.0.1"
#define SERV_PORT 8080

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
    char buff[4096];
    buff[0] = '\0';

    // send empty string 
    const char *p = " ";
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
        
        std::cout << "sending empty string" << std::endl;
        ssize_t n = write(sockfd, p, strlen(p));
        std::cout << "strlen(p) = " << strlen(p) << std::endl;
    
        n = read(sockfd, buff, 4096);
        std::cout << "n=" << n << std::endl;
        printf("%s", buff);
        close(sockfd);
    } else {
        perror("err1");
    }

    return 0;
}

