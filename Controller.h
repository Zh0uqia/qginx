#pragma once
#include <conf.h>
#include <./fastcgi/fcgi.h>
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <Response.h>
#include <RequestHandler.h>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>

class Controller
{
public:
    char* cmdGet(RequestHandler);
    char* cmdPost(RequestHandler);
    bool isPhp(std::string);
    bool isGetDynamic(RequestHandler);

private:
    char* openFile(std::string);

    void FastCgiFun(char *method, char *file_path, char *query_string, char *content)
	{
		FastCgi_init(c);
		setRequestId(c, 1);
		startConnect(c);
		sendStartRequestRecord(c); //start request body is the first request body sent  
		std::cout << "----------------------------------------php started" << std::endl;
        
        // send "name-value" pairs 
        if (method == "GET"){
		    sendParams(c, const_cast<char *>("SCRIPT_FILENAME"), file_path);
		    sendParams(c, const_cast<char *>("REQUEST_METHOD"), method);
            sendParams(c, const_cast<char *>("CONTENT_LENGTH"), const_cast<char *>("0"));
            sendParams(c, const_cast<char *>("CONTENT_TYPE"), const_cast<char *>("text/html"));
            if (query_string != NULL){
                sendParams(c, const_cast<char *>("QUERY_STRING"), query_string);
            }
        
            sendEndRequestRecord(c);// empty; same type as ENV_PARAM 
        }

        if (method == "POST"){
            size_t content_length = strlen(content);
            std::string size = std::to_string(content_length);
            char cl[size.length()+1];
            strcpy(cl, size.c_str());
            
            // std::cout << "content-length is: " << content_length << std::endl;

		    sendParams(c, const_cast<char *>("SCRIPT_FILENAME"), file_path);
		    sendParams(c, const_cast<char *>("REQUEST_METHOD"), method);
            sendParams(c, const_cast<char *>("CONTENT_LENGTH"), cl);
            sendParams(c, const_cast<char *>("CONTENT_TYPE"), const_cast<char *>("application/x-www-form-urlencoded"));

            sendEndRequestRecord(c);// empty; same type of ENV_PARAM 
            
            // make header to send body 
            // type = 5 means this is a POST request body 
            FCGI_Header t = makeHeader(5, c->requestId_, content_length, 0); 
            send(c->sockfd_, &t, sizeof(t), 0);

            // send body 
            send(c->sockfd_, content, content_length, 0);

            //make header to tell php-fpm the body ends 
            FCGI_Header endHeader;
            endHeader = makeHeader(FCGI_STDIN, c->requestId_, 0, 0);
            send(c->sockfd_, &endHeader, sizeof(endHeader), 0);

        }

    }

    char* responseFun(){
        Response resp;
        char* rp;
		rp = readFromPhp(c);

        char* html = findStartHtml(rp);

        auto htmlen = strlen(rp) - (html - rp);
		int i = 0;
        char final_html[20000];
		memset(final_html, '\0', 20000);
		
        for (i = 0; i < htmlen; i++)
		{
			final_html[i] = *(html + i);
		}
		final_html[i] = '\0';

		// std::cout << "final_html%%%%%%%%%%%%%%%%%%%%%%%%\% " << final_html << std::endl;

        char* response;
        response = resp.generateResponse(final_html);

        FastCgi_finit(c); // close the fastcgi socket

        return response;
    }

    FastCgi_t fast_cgi; // fast cgi object 
    FastCgi_t* c = &fast_cgi;
};

