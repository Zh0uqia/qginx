#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include <regex>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(){
    int i;

    for (i=0; i<4; i++){
        fork();
        printf("Hello\n");
    
    }
    return 0;
}
