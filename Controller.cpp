#include <iostream>
#include <Controller.h>

void Controller::cmd_get(Request r){
    /* interact with php */
    if (isPhp(r.getPath())){
        FastCgiFun(const_cast<char *>("GET"));
    } 
    else{
        /* send html file */
    }
}

void Controller::cmd_post(){

}

bool isPhp(std::string p){
    return true;
}
