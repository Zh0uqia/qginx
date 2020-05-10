/* An interface to initialize a simple http server.
 * bind(), start(), stop()
 */

#pragma once

class httpServer final 
{
public:
    enum class Protocal: uint8_t{
        HTTP 1.1,
        HTTP 2,
    };

    httpServer() {}
    ~httpServer() {}

    // bind socket to a port, and set socket options 
    void bindPort();

    // start listening on the port 
    void start();

    // stop listening on the port 
    void stopListening();

    // stop the server 
    void stop();

private:
    // handle necessary signals (e.g. SIGPIPE)
    std::unique_ptr<>signalHandler_;

};

