#pragma once
#include <conf.h>
#include <HttpMessage.h>
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

class Handler
{
public:
    void onRequest(std::unique_ptr<HttpMessage> headers);
    void readFile();

private:
    std::string file_;
};

