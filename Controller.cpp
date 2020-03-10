#include <iostream>
#include <Controller.h>
#include <boost/algorithm/string.hpp>

char* Controller::cmdGet(RequestHandler r){
    char* res;

    std::string fn = r.getUri();// relative path 
    fn = "/mnt/raid/webserver/" + fn; // absolute path 
    char *file_name = &fn[0];
    // int n = fn.length();
    // char file_name[n+1];

    // strcpy(file_name, fn.c_str());

    /* interact with php */
    if (isPhp(fn)){
#ifdef DEBUG 
        printf("----is php----\n");
#endif 
        if (isGetDynamic(r)){
            std::string qs = r.getQueryString();
            char *query_string = &qs[0];
            // char query_string[qs.length()+1];
            // strcpy(query_string, qs.c_str());

            FastCgiFun(const_cast<char *>("GET"), const_cast<char *>(file_name),\
                       const_cast<char *>(query_string), NULL);
        }
        else 
            FastCgiFun(const_cast<char *>("GET"), const_cast<char *>(file_name), NULL, NULL);
        
        res = responseFun();

    } 
    else{
        /* send html file */
    }

    return res;
}

char* Controller::cmdPost(RequestHandler r){
    char* res;

    std::string fn = r.getUri();
    fn = "/mnt/raid/webserver/" + fn;
    char *file_name = &fn[0];
    // int n = fn.length();
    // char file_name[n+1];

    // strcpy(file_name, fn.c_str());

    std::string ct = r.getBody();
    char *content = &ct[0];
    // int n2 = ct.length();
    // char content[n2+1];
    // strcpy(content, ct.c_str());
    
    /* interact with php */
    if (isPhp(fn)){
        printf("----is php----\n");
        FastCgiFun(const_cast<char *>("POST"), const_cast<char *>(file_name), NULL, content);
        res = responseFun();

    } 
    else{
        /* send html file */
    }

    return res;
}

bool Controller::isPhp(std::string p){
    bool res = boost::algorithm::ends_with(p, ".php");
    return res;
}

bool Controller::isGetDynamic(RequestHandler r){
    if (!r.getQueryString().empty())
        return true;
    else
        return false;
}
