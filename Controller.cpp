#include <iostream>
#include <Controller.h>
#include <boost/algorithm/string.hpp>

char* Controller::cmdGet(RequestHandler r){
    char* res;

    std::string fn = r.getUri();// relative path 
    fn = "/mnt/raid/webserver/" + fn; // absolute path 
    char *file_name = &fn[0];
    
    /* interact with php */
    if (isPhp(fn)){
#ifdef DEBUG 
        printf("----is php----\n");
#endif 
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

char* Controller::cmdPost(RequestHandler r){
    char* res;

    std::string fn = r.getUri();
    fn = "/mnt/raid/webserver/" + fn;
    char *file_name = &fn[0];
    
    std::string ct = r.getBody();
    char *content = &ct[0];
    
    /* interact with php */
    if (isPhp(fn)){
        printf("----is php----\n");
        FastCgiFun(const_cast<char *>("POST"), const_cast<char *>(file_name), NULL, content);
        res = responseFun();

    } 
    else{
        res = openFile(fn);
    }

    return res;
}

char* Controller::openFile(std::string fn){
    char* res;
    struct stat sbuf;
    std::string fileBuf;
    stat(fn.c_str(), &sbuf);

    int src_fd = open(fn.c_str(), O_RDONLY, 0);
    if (src_fd < 0) {
        std::cout << "404: failure of opening file" << std::endl;
    }
    void *mmapRet = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    close(src_fd);
    if (mmapRet == (void *)-1) {
        munmap(mmapRet, sbuf.st_size);
        std::cout << "404: failure of opening file" << std::endl;
    }
    char *src_addr = static_cast<char *>(mmapRet);
    fileBuf += std::string(src_addr, src_addr + sbuf.st_size);
    munmap(mmapRet, sbuf.st_size);
    
    char* content = (char*) malloc(10000 * sizeof(char));
    strcpy(content, fileBuf.c_str());

    Response resp;
    res = resp.generateResponse(content);
    
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
