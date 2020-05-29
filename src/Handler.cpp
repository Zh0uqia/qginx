#include <iostream>
#include <Handler.h>

void Handler::detach(){
    delete this;
}

void Handler::onRequest(connection_t* c, HttpMessage* headers){
    file_ = headers->getFilePath();

    readFile(c);

}

void Handler::readFile(connection_t* c){
    std::string buf;

    struct stat sbuf;
    stat(file_.c_str(), &sbuf);

    // consider add lock
    // if multiple process cannot open the same file concurrently
    // TODO: read Nginx this part 
    int src_fd = open(file_.c_str(), O_RDONLY, 0);

    if (src_fd < 0) {
        ResponseBuilder().status(404, "Not Found")
           .body("Could not find")
           .send(c->fd);
        return;
    }

    void *mmapRet = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    close(src_fd);

    if (mmapRet == (void *)-1) {
        munmap(mmapRet, sbuf.st_size);
        ResponseBuilder().status(404, "Not Found")
           .body("Could not find")
           .send(c->fd);
        return;
    }

    char *src_addr = static_cast<char *>(mmapRet);
    buf += std::string(src_addr, src_addr + sbuf.st_size);
    munmap(mmapRet, sbuf.st_size);

    dbPrint("Response body is: " << buf << std::endl);

    // send message back to client
    // if consider IO blocking, we should put write event into epoll here 
    ResponseBuilder().status(200, "OK")
        .body(buf)
        .send(c->fd);

}

