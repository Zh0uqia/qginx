#include <iostream>
#include <SimpleController.h>

Handler* SimpleController::getRequestHandler(HttpMessage* msg){
    return new Handler(); // remember to free it 
}

