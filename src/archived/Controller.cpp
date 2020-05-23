#include <iostream>
#include <archived/Controller.h>
#include <boost/algorithm/string.hpp>

std::string Controller::cmdGet(RequestHandler r){
    std::string res;

    std::string fn = r.getUri();// relative path 
    fn = "/mnt/raid/simple-web-server/src/" + fn; // absolute path 
    char *file_name = &fn[0];
    
    /* interact with php */
    if (isPhp(fn)){
        if (isGetDynamic(r)){
            std::string qs = r.getQueryString();
            char *query_string = &qs[0];

            FastCgiFun(const_cast<char *>("GET"), const_cast<char *>(file_name),\
                       const_cast<char *>(query_string), NULL);
        }
        else 
            FastCgiFun(const_cast<char *>("GET"), const_cast<char *>(file_name), NULL, NULL);
        
        res = responseFun();

    } 
    else{
        res = openFile(fn);
    }

    return res;
}

std::string Controller::cmdPost(RequestHandler r){
    std::string res;

    std::string fn = r.getUri();
    fn = "/mnt/raid/webserver/" + fn;
    char *file_name = &fn[0];
    
    std::string ct = r.getBody();
    char *content = &ct[0];
    
    /* interact with php */
    if (isPhp(fn)){
        FastCgiFun(const_cast<char *>("POST"), const_cast<char *>(file_name), NULL, content);
        res = responseFun();
    } 
    else{
        res = openFile(fn);
    }

    return res;
}

std::string Controller::openFile(std::string fn){
    std::string res;
    struct stat sbuf;
    std::string fileBuf;
    stat(fn.c_str(), &sbuf);

    int src_fd = open(fn.c_str(), O_RDONLY, 0);
    if (src_fd < 0) {
        std::perror("404: failure of opening file");
        res = "failure";
        return res;
    }
    void *mmapRet = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    close(src_fd);
    if (mmapRet == (void *)-1) {
        munmap(mmapRet, sbuf.st_size);
        std::perror("404: failure of opening file");
        res = "failure";
        return res;
    }
    char *src_addr = static_cast<char *>(mmapRet);
    fileBuf += std::string(src_addr, src_addr + sbuf.st_size);
    munmap(mmapRet, sbuf.st_size);
    
    res = fileBuf;
    
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
