/* An interface to initialize a simple http server.
 * bind(), start(), stop()
 */

#pragma once
#include <httpServer/HttpServerOptions.h>
#include <lib/http/httpCodec/HttpCodecFactory.h>
#include <lib/services/Bootstrap.h>

#include <functional>
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>   
#include <stdio.h> 
#include <string.h>          // For memset()
#include <memory>            // For shared_ptr<>

class HttpServer final 
{
public:
    enum class Protocal: uint8_t{
        HTTP,
        HTTP2,
    };
    
    struct IPConfig{
        std::string address;
        int port;
        Protocal protocal;
        // initialized as a nullptr 
        std::shared_ptr<HttpCodecFactory> codecFactory;
    };

    HttpServer(HttpServerOptions options);
    // ~httpServer();

    // bind socket to a port, and set socket options 
    void bindPort(IPConfig const& addrs);

    void startTcpServer();

    // start listening on the port 
    void start(std::function<void()> onSuccess = nullptr, 
               std::function<void(std::exception_ptr)> onError = nullptr);

    // stop listening on the port 
    void stopListening();

    // stop the server 
    void stop();

    // return a file descriptor associated to a listening socket 
    int getListenSocket() const;

private:
    bool started_;
    int serverFd_;
    struct sockaddr_in socketAddress_;
    
    HttpServerOptions options_;

    // handle necessary signals (e.g. SIGPIPE)
    // SignalHandler signalHandler_;
    
    IPConfig address_;

    // callback function for session create or destruction 
    // HttpSession::infoCallback* sessionInfoCb_{nullptr};

    // MasterProcess masterProcess_;
};

