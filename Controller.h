#pragma once
#include <conf.h>
#include <Request.h>
#include <./fastcgi/fcgi.h>

class Controller
{
public:
    Controller();
    ~Controller();
    
    void cmd_get();
    void cmd_post();
    bool isPhp();

    /* param: method type, file name */
    fastCgiFun(char* method, char* filename){
        FastCgi_init(&fast_cgi);
        setRequestId(&fast_cgi, 1);
		startConnect(&fast_cgi);
		sendStartRequestRecord(&fast_cgi);

        std::cout << "---------------php file--------------" << std::endl;

        sendParams(&fast_cgi, const_cast<char *>("SCRIPT_FILENAME"), filename);
		sendParams(&fast_cgi, const_cast<char *>("REQUEST_METHOD"), method);
        
        sendEndRequestRecord(&fast_cgi);

    }

private:
    FastCgi_t fast_cgi; // fast cgi object 
};

