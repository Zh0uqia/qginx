#pragma once
#include <conf.h>
#include <Request.h>
#include <./fastcgi/fcgi.h>

class Controller
{
public:
    void cmd_get(Request);
    void cmd_post();
    bool isPhp(std::string);

private:
 
    void FastCgiFun(char *method, char *file_path)
	{
		FastCgi_init(&fast_cgi);
		setRequestId(&fast_cgi, 1);
		startConnect(&fast_cgi);
		sendStartRequestRecord(&fast_cgi);
		std::cout << "----------------------------------------php started" << std::endl;

		sendParams(&fast_cgi, const_cast<char *>("SCRIPT_FILENAME"), file_path);
		sendParams(&fast_cgi, const_cast<char *>("REQUEST_METHOD"), method);
        sendParams(&fast_cgi, "CONTENT_LENGTH", "0");
        sendParams(&fast_cgi, "CONTENT_TYPE", "text/html");

        sendEndRequestRecord(&fast_cgi);
    }

    void responseGet(){
		readFromPhp(&fast_cgi);
        printf("got php file\n");

        FastCgi_finit(&(Controller::fast_cgi)); // close the fastcgi socket
    }

    FastCgi_t fast_cgi; // fast cgi object 
};

