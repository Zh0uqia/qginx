#pragma once
#include <conf.h>
#include <Core.h>
#include <HttpMessage.h>
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ResponseBuilder.h>
#include <unistd.h>

class HttpMessage;
class Handler
{
public:
    void onRequest(connection_t* c, HttpMessage* headers);
    void readFile(connection_t* c);
    void detach();

private:
    std::string file_;
};

