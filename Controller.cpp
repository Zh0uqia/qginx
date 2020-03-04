#include <iostream>
#include <Controller.h>
#include <bits/stdc++.h>

void Controller::cmd_get(Request r){
    std::string fn = r.getPath();
    int n = fn.length();
    char file_name[n+1];

    strcpy(file_name, fn.c_str());

    /* interact with php */
    if (isPhp(fn)){
        printf("----is php----\n");
        FastCgiFun(const_cast<char *>("GET"), const_cast<char *>(file_name));
        responseGet();

    } 
    else{
        /* send html file */
    }
}

void Controller::cmd_post(){

}

bool Controller::isPhp(std::string p){
    return true;
}
